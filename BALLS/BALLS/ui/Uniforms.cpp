#include "precompiled.hpp"
#include "ui/Uniforms.hpp"
#include "util/Logging.hpp"

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QWheelEvent>

#include "util/Trackball.hpp"
#include "util/TypeInfo.hpp"
#include "util/Util.hpp"

// TODO Should implement  Qtilities::Core::Interfaces::IModificationNotifier
namespace balls {

constexpr float ZOOM_INTERVAL = 1.0f;

using balls::util::types::info;
using balls::util::resolveGLType;
using balls::util::types::UniformInfo;
using balls::util::types::UniformCollection;

Uniforms::Uniforms(QObject* parent) noexcept :
QObject(parent),
        _view(glm::translate(vec3(0, 0, -8))),
        _nearPlane(.01f),
        _farPlane(100),
        _canvasSize(1, 1),
        _lastCanvasSize(1, 1),
        _meta(metaObject())
{
  setFov(glm::radians(45.0f));
  _elapsedTime.start();
}

void Uniforms::receiveUniforms(const UniformCollection& uniforms) noexcept {
  // We must consider three cases:
  // 1: Uniforms that were removed (old - new)
  // 2: Uniforms that were added (new - old)
  // 3: Uniforms we still have (new & old)

  using balls::util::types::info;
  using balls::util::resolveGLType;
  using balls::util::types::UniformInfo;
  using balls::util::types::UniformCollection;
  using std::set_difference;
  using std::set_intersection;

  UniformCollection temp;

  auto cb = _uniformList.cbegin();
  auto ce = _uniformList.cend();
  // It's okay to cache these iterators, as we're not modifying _uniformList

  // First handle discarded uniforms (old - new)
  set_difference(
    cb, ce,
    uniforms.cbegin(), uniforms.cend(),
    std::inserter(temp, temp.begin())
  );

  _handleDiscardedUniforms(temp);
  temp.clear();

  // Now handle new uniforms; we have to add these (new - old)
  set_difference(
    uniforms.cbegin(), uniforms.cend(),
    cb, ce,
    std::inserter(temp, temp.begin())
  );
  _handleNewUniforms(temp);

  temp.clear();

  // Finally, handle uniforms that still exist, but may have changed type (new & old)
  set_intersection(
    uniforms.cbegin(), uniforms.cend(),
    cb, ce,
    std::inserter(temp, temp.begin())
  );

  _handleKeptUniforms(temp);

  this->_uniformList = uniforms;
}

void Uniforms::_handleDiscardedUniforms(const UniformCollection& temp)
noexcept {
  qCDebug(logs::uniform::Name) << temp.size() << "discarded uniforms";

  for (const UniformInfo& i : temp) {
    // For each uniform we're throwing away...
    QByteArray name = i.name.toLocal8Bit();
    const char* name_cstr = name.data();

    if (_meta->indexOfProperty(name_cstr) == -1) {
      // If this is a custom uniform...

      Q_ASSERT(property(name_cstr).isValid());
      this->setProperty(name_cstr, QVariant());
      // Then clear it
      Q_ASSERT(!property(name_cstr).isValid());

      qCDebug(logs::uniform::Name) << "Removed" << i.name;
    }
  }
}

void Uniforms::_handleNewUniforms(const UniformCollection& temp) noexcept {
  qCDebug(logs::uniform::Name) << temp.size() << "new uniforms";

  for (const UniformInfo& i : temp) {
    // For each uniform we're adding...

    QByteArray name = i.name.toLocal8Bit();
    const char* name_cstr = name.data();
    auto qtype = info[i.type].qMetaType;

    if (_meta->indexOfProperty(name_cstr) == -1) {
      // If this is a custom uniform...

      qCDebug(logs::uniform::Name)
          << "Discovered custom" << QMetaType::typeName(qtype) << i.name;

      Q_ASSERT(!property(name_cstr).isValid());
      this->setProperty(name_cstr, QVariant(qtype, nullptr));
      Q_ASSERT(property(name_cstr).isValid());
      // Add a default-constructed uniform
    }
    else {
      qCDebug(logs::uniform::Name)
          << "Now using built-in" << QMetaType::typeName(qtype)
          << i.name;
    }
  }
}

void Uniforms::_handleKeptUniforms(const UniformCollection& temp) noexcept {
  qCDebug(logs::uniform::Name) << temp.size() << "uniforms carried over";

  for (const UniformInfo& i : temp) {
    // For each uniform we still have...

    QByteArray name = i.name.toLocal8Bit();
    const char* name_cstr = name.data();

    const util::types::TypeInfo& inf = info[i.type];

    QVariant prop = this->property(name_cstr);

    if (_meta->indexOfProperty(name_cstr) == -1) {
      // If this is a custom uniform...

      if (prop.userType() == inf.qMetaType) {
        // If this uniform still has the same type as before...
        qCDebug(logs::uniform::Name)
            << "Left" << prop.typeName() << i.name << "unchanged";
      }
      else {
        // This uniform is defined with a different type this time...

        if (prop.canConvert(inf.qMetaType)) {
          // If a conversion exists to this new type...
          prop.convert(inf.qMetaType);
        }
        else {
          prop = QVariant(inf.qMetaType, nullptr);
          // Otherwise, just default-construct a value
        }

        this->setProperty(name_cstr, prop);
        qCDebug(logs::uniform::Name)
            << "Have" << prop.typeName() << i.name << "but it's now a"
            << QMetaType::typeName(info[i.type].qMetaType) << i.name;
      }
    }
  }
}

void Uniforms::resetModelView() noexcept {
  _model = mat4();
  _view = mat4();
}

void Uniforms::setFov(const float fov) noexcept {
  _fov = fov;

  _updateProjection();
}

void Uniforms::_updateProjection() noexcept {
  _projection = glm::perspectiveFov(
    _fov,
    static_cast<float>(_canvasSize.x),
    static_cast<float>(_canvasSize.y + 1),
    _nearPlane,
    _farPlane
  );
}

bool Uniforms::event(QEvent* e) {
  return false;
}

bool Uniforms::eventFilter(QObject* obj, QEvent* event) {
  switch (event->type()) {
  case QEvent::MouseMove:
    mouseMoveEvent(static_cast<QMouseEvent*>(event));
    return false;

  case QEvent::MouseButtonPress:
    mousePressEvent(static_cast<QMouseEvent*>(event));
    return false;

  case QEvent::Wheel:
    wheelEvent(static_cast<QWheelEvent*>(event));
    return false;

  case QEvent::Resize:
    resizeEvent(static_cast<QResizeEvent*>(event));
    return false;

  default:
    return false;
  }
}


void Uniforms::mouseMoveEvent(QMouseEvent* e) noexcept {
  this->_mousePos.x = e->x();
  this->_mousePos.y = e->y();

  vec2 localPos = { e->localPos().x(), e->localPos().y() };

  if (e->buttons() & Qt::LeftButton) {
    // If the left mouse button is being held down...

    vec2 delta = localPos - vec2(_lastMousePos);
    _model = glm::rotate(_model, glm::radians(delta.x), vec3(0, 1, 0));
    _model = glm::rotate(_model, glm::radians(delta.y), vec3(1, 0, 0));
  }

  this->_lastMousePos = localPos;
}

void Uniforms::mousePressEvent(QMouseEvent* e) noexcept {

  if (e->buttons() & Qt::LeftButton) {
    // If the left mouse button was just clicked...

    _trackball.click(
      e->x(),
      _canvasSize.y - e->y()
    );
  }

}

void Uniforms::wheelEvent(QWheelEvent* e) noexcept {
  int delta = e->angleDelta().y();
  //this->setFov(_fov + ((delta > 0) ? ZOOM_INTERVAL : -ZOOM_INTERVAL));
  _view = glm::translate(_view, {0.0f, 0.0f, (delta > 0 ? ZOOM_INTERVAL : -ZOOM_INTERVAL)});
}

void Uniforms::resizeEvent(QResizeEvent* e) noexcept {
  const QSize& size = e->size();
  int w = size.width();
  int h = size.height();

  _canvasSize.x = w;
  _canvasSize.y = h;

  const QSize& lastSize = e->oldSize();
  _lastCanvasSize.x = lastSize.width();
  _lastCanvasSize.y = lastSize.height();

  _updateProjection();
  _trackball.setBounds(w, h);
}
}
