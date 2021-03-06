#ifndef VECTOR4PROPERTY_HPP
#define VECTOR4PROPERTY_HPP

#include <glm/fwd.hpp>

#include "ui/property/VectorProperty.hpp"
#include "Constants.hpp"

namespace balls {

using glm::bvec4;
using glm::dvec4;
using glm::ivec4;
using glm::vec4;
using glm::uvec4;

extern template class VectorProperty<ivec4>;
extern template class VectorProperty<bvec4>;
extern template class VectorProperty<dvec4>;
extern template class VectorProperty<vec4>;
extern template class VectorProperty<uvec4>;

class BVec4Property : public VectorProperty<bvec4> {
  Q_OBJECT

  Q_PROPERTY(bool x READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(bool y READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(bool z READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(bool w READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using VectorProperty<bvec4>::VectorProperty;
protected:
  const QRegularExpression& regex() const noexcept override { return constants::regex::BVEC4; }
};

class IVec4Property : public VectorProperty<ivec4> {
  Q_OBJECT

  Q_PROPERTY(int x READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(int y READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(int z READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(int w READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using VectorProperty<ivec4>::VectorProperty;
protected:
  const QRegularExpression& regex() const noexcept override { return constants::regex::IVEC4; }
};

class UVec4Property : public VectorProperty<uvec4> {
  Q_OBJECT

  Q_PROPERTY(unsigned int x READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(unsigned int y READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(unsigned int z READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(unsigned int w READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using VectorProperty<uvec4>::VectorProperty;
protected:
  const QRegularExpression& regex() const noexcept override { return constants::regex::UVEC4; }
};

class Vec4Property : public VectorProperty<vec4> {
  Q_OBJECT

  Q_PROPERTY(float x READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(float y READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(float z READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(float w READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using VectorProperty<vec4>::VectorProperty;
protected:
  const QRegularExpression& regex() const noexcept override { return constants::regex::VEC4; }
};

class DVec4Property : public VectorProperty<dvec4> {
  Q_OBJECT

  Q_PROPERTY(double x READ (_get<0>) WRITE (_set<0>) DESIGNABLE true FINAL)
  Q_PROPERTY(double y READ (_get<1>) WRITE (_set<1>) DESIGNABLE true FINAL)
  Q_PROPERTY(double z READ (_get<2>) WRITE (_set<2>) DESIGNABLE true FINAL)
  Q_PROPERTY(double w READ (_get<3>) WRITE (_set<3>) DESIGNABLE true FINAL)

public:
  using VectorProperty<dvec4>::VectorProperty;
protected:
  const QRegularExpression& regex() const noexcept override { return constants::regex::VEC4; }
};
}
#endif // VECTOR4PROPERTY_HPP
