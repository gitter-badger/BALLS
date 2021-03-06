#ifndef BALLSCANVAS_HPP
#define BALLSCANVAS_HPP

#include <unordered_map>

#include <QtCore/QtGlobal>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLDebugLogger>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtWidgets/QOpenGLWidget>

#include "mesh/Mesh.hpp"
#include "shader/ShaderInputs.hpp"
#include "shader/ShaderUniform.hpp"
#include "config/Settings.hpp"
#include "util/Logging.hpp"
#include "util/Trackball.hpp"
#include "util/TypeInfo.hpp"
#include "ui/Uniforms.hpp"

class QOpenGLFunctions_3_0;
class QOpenGLFunctions_3_1;
class QOpenGLFunctions_3_2_Core;
class QOpenGLFunctions_3_3_Core;
class QOpenGLFunctions_4_0_Core;
class QOpenGLFunctions_4_1_Core;
class QOpenGLFunctions_4_2_Core;
class QOpenGLFunctions_4_3_Core;
class QOpenGLShader;

namespace balls {
namespace mesh {
class MeshGenerator;
}


using std::pair;
using std::unordered_map;
using std::uint8_t;
using namespace balls::config;
using namespace balls::shader;
using balls::util::types::UniformCollection;
using balls::util::types::UniformInfo;

constexpr GLuint DEFAULT_INDEX = -1;
constexpr GLenum DEFAULT_TYPE = -1;
constexpr GLint DEFAULT_SIZE = 0;


class BallsCanvas : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  explicit BallsCanvas(QWidget* parent = nullptr);
  ~BallsCanvas();
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(const int, const int) override;
  void setMesh(mesh::MeshGenerator*) noexcept;
  bool updateShaders(const QString&, const QString&, const QString&) noexcept;
public /* getters/setters */:
  QOpenGLShaderProgram& getShader() noexcept { return _shader; }
  const QOpenGLShaderProgram& getShader() const noexcept { return _shader;  }

  QOpenGLDebugLogger& getLogger() noexcept { return _log; }
  const QOpenGLDebugLogger& getLogger() const noexcept { return _log; }

  uint8_t getOpenGLMajor() const noexcept { return _glmajor; }
  uint8_t getOpenGLMinor() const noexcept { return _glminor; }

  const Uniforms& getUniforms() const noexcept { return _uniforms; }
  Uniforms& getUniforms() noexcept { return _uniforms; }
signals:
  void geometryShadersSupported(const bool);
  void gl3NotSupported();
  void finishedInitializing();
  void fatalGraphicsError(const QString&, const QString&, const int);
  void graphicsWarning(const QString&, const QString&);

  void uniformsDiscovered(const UniformCollection&);
public slots:
  void setOption(const bool) noexcept;
  void setOption(const int) noexcept;
  void resetCamera() noexcept;
public:
  void setUniform(const UniformInfo&, const QVariant&) noexcept;
protected:
  void mouseMoveEvent(QMouseEvent *e) override;
  void wheelEvent(QWheelEvent *) override;
  void timerEvent(QTimerEvent *) override;
private /* mesh information */:
  mesh::MeshGenerator* _meshgen;
  mesh::Mesh _mesh;

private /* shader attributes/uniforms */:
  Uniforms _uniforms;
  const QMetaObject* _uniformsMeta;
  int _uniformsPropertyOffset;
  int _uniformsPropertyCount;

  unordered_map<AttributeName, int> _attributes;
  unordered_map<QString, Setting> _settings;
private /* OpenGL structures */:
  QOpenGLDebugLogger _log;
  QOpenGLBuffer _vbo;
  QOpenGLBuffer _ibo;
  QOpenGLVertexArrayObject _vao;
  QOpenGLShaderProgram _shader;
  uint8_t _glmajor : 3;
  uint8_t _glminor : 3;

private /* OpenGL function pointers */:
  QOpenGLFunctions_3_0* _gl30;
  QOpenGLFunctions_3_1* _gl31;
  QOpenGLFunctions_3_2_Core* _gl32;
  QOpenGLFunctions_3_3_Core* _gl33;
  QOpenGLFunctions_4_0_Core* _gl40;
  QOpenGLFunctions_4_1_Core* _gl41;
  QOpenGLFunctions_4_2_Core* _gl42;
  QOpenGLFunctions_4_3_Core* _gl43;

private /* update methods */:
  void _updateUniformList() noexcept;
  void _updateUniformValues() noexcept;
private /* initializers */:
  void _initAttributeLocations() noexcept;
  void _initSettings() noexcept;
  void _initGLPointers();
  void _initGLMemory();
  void _initLogger() noexcept;
  void _initShaders() noexcept ;
  void _initAttributes() noexcept;

private /* templated utility methods */:
  template <GLenum E>
  void _glToggle(const bool enabled) noexcept {
    if (enabled) {
      this->glEnable(E);
    }
    else {
      this->glDisable(E);
    }
  }

  template <GLenum GL>
  void _updateGLSetting(const QString& key) noexcept {
    Setting& setting = _settings[key];

    if (setting.changed) {
      this->_glToggle<GL>(setting.value.toBool());
      setting.changed = false;
    }
  }
};
}

#endif // BALLSCANVAS_HPP
