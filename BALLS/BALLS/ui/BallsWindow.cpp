﻿#include "precompiled.hpp"
#include "ui/BallsWindow.hpp"

#include <QtCore/QMetaEnum>
#include <QtCore/QSettings>
#include <QtWidgets/QErrorMessage>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include "ui/QsciLexerGLSL.h"
#include "exception/FileException.hpp"
#include "exception/JsonException.hpp"
#include "Constants.hpp"
#include "config/ProjectConfig.hpp"
#include "mesh/MeshFunction.hpp"
#include "mesh/MeshGenerator.hpp"
#include "mesh/Generators.hpp"
#include "util/Util.hpp"
#include "shader/ShaderUniform.hpp"

Q_DECLARE_METATYPE(balls::mesh::MeshGenerator*)

namespace balls {

using namespace constants;

constexpr QSettings::Scope SCOPE = QSettings::UserScope;
constexpr QSettings::Format FORMAT = QSettings::NativeFormat;

BallsWindow::BallsWindow(QWidget* parent) noexcept :
QMainWindow(parent),
            _generatorsInitialized(false),
            _vertLexer(new QsciLexerGLSL(this)),
            _fragLexer(new QsciLexerGLSL(this)),
            _geomLexer(new QsciLexerGLSL(this)),
            _save(new QFileDialog(this, tr("Save BALLS project"), ".")),
            _load(new QFileDialog(this, tr("Load BALLS project"), ".")),
            _error(new QErrorMessage(this)),
_settings(new QSettings(this)) {
  ui.setupUi(this);

  ui.vertexEditor->setLexer(_vertLexer);
  ui.fragmentEditor->setLexer(_fragLexer);
  ui.geometryEditor->setLexer(_geomLexer);

  ProjectConfig config = balls::config::loadFromFile(":/example/default.balls");
  ui.vertexEditor->setText(config.vertexShader);
  ui.fragmentEditor->setText(config.fragmentShader);

  _save->setAcceptMode(QFileDialog::AcceptSave);
  _save->setDefaultSuffix(extensions::BALLS);
  _save->setFileMode(QFileDialog::FileMode::AnyFile);
  _save->setNameFilters({filters::BALLS});
  _save->setNameFilterDetailsVisible(true);

  _load->setAcceptMode(QFileDialog::AcceptOpen);
  _load->setFileMode(QFileDialog::FileMode::ExistingFile);
  _load->setNameFilterDetailsVisible(true);

  ui.uniforms->setObject(&ui.canvas->getUniforms());
  ui.uniforms->registerCustomPropertyCB(shader::createShaderProperty);
}

BallsWindow::~BallsWindow() { _settings->sync(); }

ProjectConfig BallsWindow::getProjectConfig() const noexcept {
  ProjectConfig project;
  project.vertexShader = ui.vertexEditor->text();
  project.fragmentShader = ui.fragmentEditor->text();
  project.glMajor = ui.canvas->getOpenGLMajor();
  project.glMinor = ui.canvas->getOpenGLMinor();

  const Uniforms& uniforms = ui.canvas->getUniforms();

  for (const QByteArray& u : uniforms.dynamicPropertyNames()) {
    QVariant uniform = uniforms.property(u);

    Q_ASSERT(uniform.isValid());

    project.uniforms[QString(u)] = uniform;
  }

  return project;
}

void BallsWindow::setMesh(const int index) noexcept {
  QVariant var = this->ui.meshComboBox->currentData();
  mesh::MeshGenerator* generator = var.value<mesh::MeshGenerator*>();

  Q_ASSERT(generator != nullptr);
  Q_ASSERT(0 <= index&&   index < ui.meshComboBox->count());
  Q_ASSERT(var.isValid());

  this->ui.canvas->setMesh(generator);

  qCDebug(logs::ui::Name) << "Switched to mesh" << generator->getName();
}

void BallsWindow::initializeMeshGenerators() noexcept {
  using namespace balls::mesh;

  if (Q_LIKELY(!this->_generatorsInitialized)) {
    auto _addGenerator = [this](MeshGenerator * gen) noexcept {
      QVariant var = QVariant::fromValue(static_cast<MeshGenerator*>(gen));
      ui.meshComboBox->addItem(tr(qPrintable(gen->getName())), var);

      qCDebug(logs::ui::Name) << "Added mesh generator" << gen->getName() << "to selector";
    };
    _addGenerator(&generators::quad);
    _addGenerator(&generators::box);
    _addGenerator(&generators::tetrahedron);
    _addGenerator(&generators::icosahedron);
    _addGenerator(&generators::sphere80);
    _addGenerator(&generators::sphere320);
    _addGenerator(&generators::icosphere);
    _addGenerator(&generators::cylinder);
    _addGenerator(&generators::cone);
    _addGenerator(&generators::uvsphere);
    _addGenerator(&generators::torus);
    _addGenerator(&generators::ellipsoid);

    this->forceShaderUpdate();
  }

  this->_generatorsInitialized = true;
}

void BallsWindow::forceShaderUpdate() noexcept {
  QString vertex = ui.vertexEditor->text();
  QString geometry = ui.geometryEditor->text();
  QString fragment = ui.fragmentEditor->text();
  ui.log->clear();

  ui.uniforms->setObject(&ui.canvas->getUniforms());
  ui.canvas->makeCurrent();

  if (this->ui.canvas->updateShaders(vertex, geometry, fragment))
  {
    ui.canvas->update();
    this->ui.log->appendPlainText(tr("Success"));
  }

  else {
    const QOpenGLShaderProgram& shader = this->ui.canvas->getShader();
    const QOpenGLDebugLogger& log = ui.canvas->getLogger();

    this->ui.log->appendPlainText(shader.log());
    qDebug() << shader.log();

    if (Q_LIKELY(log.isLogging())) {
      using namespace logs;

      for (const QOpenGLDebugMessage& message : log.loggedMessages()) {
        qCDebug(gl::Message) << message;
        this->ui.log->appendPlainText(message.message());
      }
    }
  }
}

void BallsWindow::saveProject() noexcept {
  _save->open(this, SLOT(_saveProject(QString)));
  qCDebug(logs::ui::Name) << "Opened save dialog...";
}

void BallsWindow::_saveProject(const QString& path) noexcept {
  try {
    balls::config::saveToFile(getProjectConfig(), path, &ui.canvas->getUniforms());
  }
  catch (const FileException& error) {
    QString e = error.fullMessage();
    qCWarning(logs::app::project::Name) << e;
    _error->showMessage(e);
  }
  catch (...) {
    qCritical() << "Unknown error";
    _error->showMessage(tr("Unknown error"));
  }
}

void BallsWindow::loadProject() {
  _load->open(this, SLOT(_loadProject(QString)));
  qCDebug(logs::ui::Name) << "Opened load dialog...";
}

void BallsWindow::_loadProject(const QString& path) noexcept {
  try {
    ProjectConfig project = balls::config::loadFromFile(path);

    if (ui.canvas->getOpenGLMajor() >= project.glMajor) {
      // If we can use this version of OpenGL...

      ui.vertexEditor->setText(project.vertexShader);
      ui.fragmentEditor->setText(project.fragmentShader);

      forceShaderUpdate();

      for (const auto& u : project.uniforms) {
        //ui.canvas->setUniform(u., u.second);
      }
    }

    qCDebug(logs::app::project::Name) << "Loaded project from" << path;
  }
  catch (const FileException& error) {
    QString e = error.fullMessage();
    qCWarning(logs::app::project::Name) << e;
    _error->showMessage(e);
  }
  catch (const JsonException& error) {
    QString e = error.fullMessage();
    qCWarning(logs::app::project::Name) << e;
    _error->showMessage(e);
  }
  catch (...) {
    qCritical() << "Unknown error";
    _error->showMessage(tr("Unknown error"));
  }
}

void BallsWindow::loadExample() noexcept {

  QObject* s = sender();
  Q_ASSERT(s != nullptr); // This should only be called from a signal

  QVariant example = s->property(properties::EXAMPLE);
  Q_ASSERT(example.type() == QVariant::String);

  QString e = prefixes::EXAMPLE + example.toString();

  _loadProject(e);
  qCDebug(logs::app::project::Name) << "Loaded example project" << e;
}

void BallsWindow::reportFatalError(const QString& title,
                                   const QString& text,
                                   const int error) noexcept {
  QMessageBox::critical(this, title, text);
  qCritical().nospace() << "Exiting with fatal error " << error << ": " << text;
  qApp->exit(error);
}

void BallsWindow::closeEvent(QCloseEvent* event)
{
  _settings->setValue("geometry", saveGeometry());
  _settings->setValue("windowState", saveState());
  QMainWindow::closeEvent(event);
}

void BallsWindow::reportWarning(const QString& title,
                                const QString& text) noexcept {
  QMessageBox::warning(this, title, text);
  qWarning() << text;
}

void BallsWindow::showAboutQt() noexcept {
  qApp->aboutQt();
}
}
