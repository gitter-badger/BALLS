#ifndef MATRIXPROPERTIES_HPP
#define MATRIXPROPERTIES_HPP

#include <QObject>
#include <QString>
#include <QPropertyEditor/Property.h>

#include <glm/fwd.hpp>

#include "ui/property/MatrixProperty.hpp"
#include "ui/property/Vector2Property.hpp"
#include "ui/property/Vector3Property.hpp"
#include "ui/property/Vector4Property.hpp"

namespace balls {

using namespace glm;

//==============================================================================
// Float matrices
//==============================================================================

extern template class MatrixProperty<mat2, Vec2Property>;
extern template class MatrixProperty<mat2x3, Vec3Property>;
extern template class MatrixProperty<mat2x4, Vec4Property>;
extern template class MatrixProperty<mat3x2, Vec2Property>;
extern template class MatrixProperty<mat3, Vec3Property>;
extern template class MatrixProperty<mat3x4, Vec4Property>;
extern template class MatrixProperty<mat4x2, Vec2Property>;
extern template class MatrixProperty<mat4x3, Vec3Property>;
extern template class MatrixProperty<mat4, Vec4Property>;

// 2-column matrices ///////////////////////////////////////////////////////////
class Mat2Property : public MatrixProperty<mat2, Vec2Property> {
  Q_OBJECT

  Q_PROPERTY(vec2 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec2 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<mat2, Vec2Property>::MatrixProperty;
};

class Mat2x3Property : public MatrixProperty<mat2x3, Vec3Property> {
  Q_OBJECT

  Q_PROPERTY(vec3 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec3 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<mat2x3, Vec3Property>::MatrixProperty;
};

class Mat2x4Property : public MatrixProperty<mat2x4, Vec4Property> {
  Q_OBJECT

  Q_PROPERTY(vec4 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec4 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<mat2x4, Vec4Property>::MatrixProperty;
};
// /////////////////////////////////////////////////////////////////////////////

// 3-column matrices ///////////////////////////////////////////////////////////
class Mat3x2Property : public MatrixProperty<mat3x2, Vec2Property> {
  Q_OBJECT

  Q_PROPERTY(vec2 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec2 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec2 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<mat3x2, Vec2Property>::MatrixProperty;
};

class Mat3Property : public MatrixProperty<mat3, Vec3Property> {
  Q_OBJECT

  Q_PROPERTY(vec3 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec3 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec3 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<mat3, Vec3Property>::MatrixProperty;
};

class Mat3x4Property : public MatrixProperty<mat3x4, Vec4Property> {
  Q_OBJECT

  Q_PROPERTY(vec4 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec4 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec4 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<mat3x4, Vec4Property>::MatrixProperty;
};
// /////////////////////////////////////////////////////////////////////////////

// 4-column matrices ///////////////////////////////////////////////////////////
class Mat4x2Property : public MatrixProperty<mat4x2, Vec2Property> {
  Q_OBJECT

  Q_PROPERTY(vec2 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec2 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec2 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec2 col3 READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<mat4x2, Vec2Property>::MatrixProperty;
};

class Mat4x3Property : public MatrixProperty<mat4x3, Vec3Property> {
  Q_OBJECT

  Q_PROPERTY(vec3 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec3 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec3 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec3 col3 READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<mat4x3, Vec3Property>::MatrixProperty;
};

class Mat4Property : public MatrixProperty<mat4, Vec4Property> {
  Q_OBJECT

  Q_PROPERTY(vec4 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec4 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec4 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(vec4 col3 READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<mat4, Vec4Property>::MatrixProperty;
};
// /////////////////////////////////////////////////////////////////////////////



//==============================================================================
// Double matrices
//==============================================================================

extern template class MatrixProperty<dmat2, DVec2Property>;
extern template class MatrixProperty<dmat2x3, DVec3Property>;
extern template class MatrixProperty<dmat2x4, DVec4Property>;
extern template class MatrixProperty<dmat3x2, DVec2Property>;
extern template class MatrixProperty<dmat3, DVec3Property>;
extern template class MatrixProperty<dmat3x4, DVec4Property>;
extern template class MatrixProperty<dmat4x2, DVec2Property>;
extern template class MatrixProperty<dmat4x3, DVec3Property>;
extern template class MatrixProperty<dmat4, DVec4Property>;

// 2-column matrices ///////////////////////////////////////////////////////////
class DMat2Property : public MatrixProperty<dmat2, DVec2Property> {
  Q_OBJECT

  Q_PROPERTY(dvec2 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec2 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<dmat2, DVec2Property>::MatrixProperty;
};

class DMat2x3Property : public MatrixProperty<dmat2x3, DVec3Property> {
  Q_OBJECT

  Q_PROPERTY(dvec3 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec3 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<dmat2x3, DVec3Property>::MatrixProperty;
};

class DMat2x4Property : public MatrixProperty<dmat2x4, DVec4Property> {
  Q_OBJECT

  Q_PROPERTY(dvec4 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec4 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<dmat2x4, DVec4Property>::MatrixProperty;
};
// /////////////////////////////////////////////////////////////////////////////

// 3-column matrices ///////////////////////////////////////////////////////////
class DMat3x2Property : public MatrixProperty<dmat3x2, DVec2Property> {
  Q_OBJECT

  Q_PROPERTY(dvec2 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec2 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec2 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<dmat3x2, DVec2Property>::MatrixProperty;
};

class DMat3Property : public MatrixProperty<dmat3, DVec3Property> {
  Q_OBJECT

  Q_PROPERTY(dvec3 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec3 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec3 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<dmat3, DVec3Property>::MatrixProperty;
};

class DMat3x4Property : public MatrixProperty<dmat3x4, DVec4Property> {
  Q_OBJECT

  Q_PROPERTY(dvec4 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec4 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec4 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<dmat3x4, DVec4Property>::MatrixProperty;
};
// /////////////////////////////////////////////////////////////////////////////

// 4-column matrices ///////////////////////////////////////////////////////////
class DMat4x2Property : public MatrixProperty<dmat4x2, DVec2Property> {
  Q_OBJECT

  Q_PROPERTY(dvec2 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec2 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec2 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec2 col3 READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<dmat4x2, DVec2Property>::MatrixProperty;
};

class DMat4x3Property : public MatrixProperty<dmat4x3, DVec3Property> {
  Q_OBJECT

  Q_PROPERTY(dvec3 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec3 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec3 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec3 col3 READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<dmat4x3, DVec3Property>::MatrixProperty;
};

class DMat4Property : public MatrixProperty<dmat4, DVec4Property> {
  Q_OBJECT

  Q_PROPERTY(dvec4 col0 READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec4 col1 READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec4 col2 READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(dvec4 col3 READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using MatrixProperty<dmat4, DVec4Property>::MatrixProperty;
};
// /////////////////////////////////////////////////////////////////////////////

}

#endif // MATRIXPROPERTIES_HPP
