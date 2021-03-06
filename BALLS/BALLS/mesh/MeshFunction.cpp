#include "precompiled.hpp"
#include "mesh/MeshFunction.hpp"

#include "mesh/Mesh.hpp"
#include "mesh/MeshParameter.hpp"
#include "mesh/MeshGenerator.hpp"
#include "util/Util.hpp"

namespace balls {
namespace mesh {

const QString HEIGHT = "height";
const QString INNER_RADIUS = "inner-radius";
const QString LENGTH = "length";
const QString OUTER_RADIUS = "outer-radius";
const QString RADIUS = "radius";
const QString RESOLUTION = "resolution";
const QString SUBDIVISIONS = "subdivisions";
const QString U_SAMPLES = "u-samples";
const QString V_SAMPLES = "v-samples";
const QString WIDTH = "width";
const QString X_SCALE = "x-scale";
const QString Y_SCALE = "y-scale";
const QString Z_SCALE = "z-scale";

namespace functions {

using std::cos;
using std::max;
using std::min;
using std::sin;
using std::sqrt;
using std::uint64_t;
using std::unordered_map;

const float PHI = (1 + sqrt(5)) / 2.0f;

MeshFunction empty = [](const MeshParameters&) { return Mesh(); };

MeshFunction quad = [](const MeshParameters& params) {
  float length = params.at(LENGTH).get<float>();
  float width = params.at(WIDTH).get<float>();

  Mesh mesh;

  mesh.add_vertex(width / 2.0f, -length / 2.0f, 0.0f);
  mesh.add_vertex(width / 2.0f, length / 2.0f, 0.0f);
  mesh.add_vertex(-width / 2.0f, length / 2.0f, 0.0f);
  mesh.add_vertex(-width / 2.0f, -length / 2.0f, 0.0f);

  mesh.add_faces({{1, 2, 0}, {2, 3, 0}});
  return mesh;
};

MeshFunction box = [](const MeshParameters& params) {
  float length = params.at(LENGTH).get<float>();
  float width = params.at(WIDTH).get<float>();
  float height = params.at(HEIGHT).get<float>();

  float hl = length / 2;
  float hw = width / 2;
  float hh = height / 2;

  Mesh mesh;

  mesh.add_vertex(hw, -hl, -hh);
  mesh.add_vertex(hw, -hl, hh);
  mesh.add_vertex(-hw, -hl, hh);
  mesh.add_vertex(-hw, -hl, -hh);
  mesh.add_vertex(hw, hl, -hh);
  mesh.add_vertex(hw, hl, hh);
  mesh.add_vertex(-hw, hl, hh);
  mesh.add_vertex(-hw, hl, -hh);

  mesh.add_faces({
    {0, 2, 3},
    {0, 1, 2},
    {4, 1, 0},
    {4, 5, 1},
    {7, 5, 4},
    {7, 6, 5},
    {3, 6, 7},
    {3, 2, 6},
    {1, 6, 2},
    {1, 5, 6},
    {3, 7, 4},
    {3, 4, 0}
  });
  // Indices derived from a sketch I made on paper
  return mesh;
};

MeshFunction tetrahedron = [](const MeshParameters& params) {
  float length = params.at(LENGTH).get<float>();

  float hl = length / 2;
  Mesh mesh;

  mesh.add_vertex(hl, hl, hl);
  mesh.add_vertex(hl, -hl, -hl);
  mesh.add_vertex(-hl, hl, -hl);
  mesh.add_vertex(-hl, -hl, hl);

  mesh.add_faces({{0, 1, 2}, {0, 3, 1}, {0, 2, 3}, {2, 1, 3}});

  return mesh;
};


MeshFunction icosahedron = [](const MeshParameters& params) {
  float radius = params.at(RADIUS).get<float>();
  int subdivisions = params.at(SUBDIVISIONS).get<int>();

  Q_ASSERT(subdivisions >= 0);

  Mesh mesh;

  unordered_map<uint64_t, Mesh::IndexType> indices;
  auto makeMidpoint = [&](const Mesh::IndexType p1, const Mesh::IndexType p2) {
    const vec3& v1 = mesh[p1];
    const vec3& v2 = mesh[p2];
    vec3 mid = (v1 + v2) / 2.0f;
    return mesh.add_vertex(glm::normalize(mid) * radius);
  };

  mesh.add_vertex(glm::normalize(vec3(-1, PHI, 0)) * radius);
  mesh.add_vertex(glm::normalize(vec3(1, PHI, 0)) * radius);
  mesh.add_vertex(glm::normalize(vec3(-1, -PHI, 0)) * radius);
  mesh.add_vertex(glm::normalize(vec3(1, -PHI, 0)) * radius);
  mesh.add_vertex(glm::normalize(vec3(0, -1, PHI)) * radius);
  mesh.add_vertex(glm::normalize(vec3(0, 1, PHI)) * radius);
  mesh.add_vertex(glm::normalize(vec3(0, -1, -PHI)) * radius);
  mesh.add_vertex(glm::normalize(vec3(0, 1, -PHI)) * radius);
  mesh.add_vertex(glm::normalize(vec3(PHI, 0, -1)) * radius);
  mesh.add_vertex(glm::normalize(vec3(PHI, 0, 1)) * radius);
  mesh.add_vertex(glm::normalize(vec3(-PHI, 0, -1)) * radius);
  mesh.add_vertex(glm::normalize(vec3(-PHI, 0, 1)) * radius);

  vector<array<Mesh::IndexType, 3>> faces({
    {0, 11, 5},
    {0, 5, 1},
    {0, 1, 7},
    {0, 7, 10},
    {0, 10, 11},
    {1, 5, 9},
    {5, 11, 4},
    {11, 10, 2},
    {10, 7, 6},
    {7, 1, 8},
    {3, 9, 4},
    {3, 4, 2},
    {3, 2, 6},
    {3, 6, 8},
    {3, 8, 9},
    {4, 9, 5},
    {2, 4, 11},
    {6, 2, 10},
    {8, 6, 7},
    {9, 8, 1}
  });


  for (int i = 0; i < subdivisions; ++i) {
    decltype(faces) faces1;

    for (const array<Mesh::IndexType, 3>& face : faces) {

      Mesh::IndexType a = makeMidpoint(face[0], face[1]);
      Mesh::IndexType b = makeMidpoint(face[1], face[2]);
      Mesh::IndexType c = makeMidpoint(face[2], face[0]);

      faces1.push_back({face[0], a, c});
      faces1.push_back({face[1], b, a});
      faces1.push_back({face[2], c, b});
      faces1.push_back({a, b, c});
    }

    faces.swap(faces1);
  }

  for (const array<Mesh::IndexType, 3>& face : faces) {
    mesh.add_face(face[0], face[1], face[2]);
  }

  Q_ASSERT(mesh.indices().size() % 3 == 0);
  return mesh;
};



MeshFunction cylinder = [](const MeshParameters& params) {
  int resolution = params.at(RESOLUTION).get<int>();
  float height = params.at(HEIGHT).get<float>();
  float radius = params.at(RADIUS).get<float>();

  Q_ASSERT(resolution >= 3);

  float hh = height / 2;

  Mesh mesh;

  Mesh::IndexType topCenter = mesh.add_vertex(0, hh, 0);
  Mesh::IndexType bottomCenter = mesh.add_vertex(0, -hh, 0);

  for (int i = 0; i < resolution; ++i) {
    float angle = (static_cast<float>(i) / resolution) * (2 * M_PI);
    float opp = qFastSin(angle) * radius;
    float adj = qFastCos(angle) * radius;

    mesh.add_vertex(adj, hh, opp);
    mesh.add_vertex(adj, -hh, opp);

    Mesh::IndexType top = ((i + 1) * 2) % ((resolution * 2) + 2);
    Mesh::IndexType topNext = max(2, (top + 2) % ((resolution * 2) + 2));
    Mesh::IndexType bottom = top + 1;
    Mesh::IndexType bottomNext = topNext + 1;

    mesh.add_face(topCenter, topNext, top);
    mesh.add_face(top, bottomNext, bottom);
    mesh.add_face(top, topNext, bottomNext);
    mesh.add_face(bottomCenter, bottom, bottomNext);
  }

  return mesh;
};

MeshFunction cone = [](const MeshParameters& params) {
  int resolution = params.at(RESOLUTION).get<int>();
  float height = params.at(HEIGHT).get<float>();
  float radius = params.at(RADIUS).get<float>();

  Q_ASSERT(resolution >= 3);

  float hh = height / 2;

  Mesh mesh;

  Mesh::IndexType topCenter = mesh.add_vertex(0, hh, 0);
  Mesh::IndexType bottomCenter = mesh.add_vertex(0, -hh, 0);

  for (int i = 0; i < resolution; ++i) {
    float angle = (static_cast<float>(i) / resolution) * 2 * M_PI;
    float opp = qFastSin(angle) * radius;
    float adj = qFastCos(angle) * radius;

    Mesh::IndexType index = mesh.add_vertex(adj, -hh, opp);

    Mesh::IndexType v = i + 2;
    Mesh::IndexType vNext = max(2, (v + 1) % (resolution + 2));

    mesh.add_face(topCenter, vNext, v);
    mesh.add_face(bottomCenter, v, vNext);
  }

  return mesh;
};

MeshFunction uvsphere = [](const MeshParameters& params) {
  int u_samples = params.at(U_SAMPLES).get<int>();
  int v_samples = params.at(V_SAMPLES).get<int>();
  float x_scl = params.at(X_SCALE).get<float>();
  float y_scl = params.at(Y_SCALE).get<float>();
  float z_scl = params.at(Z_SCALE).get<float>();

  Mesh mesh;

  for (int t = 0; t < u_samples; t++) {
    float theta1 = (static_cast<float>(t) / u_samples) * 2 * M_PI;
    float theta2 = (static_cast<float>(t + 1) / u_samples) * 2 * M_PI;

    float st1 = qFastSin(theta1), st2 = qFastSin(theta2);
    float ct1 = qFastCos(theta1), ct2 = qFastCos(theta2);

    for (int p = 0; p < v_samples; p++) {
      float phi1 = (static_cast<float>(p) / v_samples) * 2 * M_PI;
      float phi2 = (static_cast<float>(p + 1) / v_samples) * 2 * M_PI;

      float sp1 = qFastSin(phi1), sp2 = qFastSin(phi2);
      float cp1 = qFastCos(phi1), cp2 = qFastCos(phi2);

      auto v1 = mesh.add_vertex(x_scl * st1 * cp1, y_scl * st1 * sp1, z_scl * ct1);
      auto v2 = mesh.add_vertex(x_scl * st1 * cp2, y_scl * st1 * sp2, z_scl * ct1);
      auto v3 = mesh.add_vertex(x_scl * st2 * cp2, y_scl * st2 * sp2, z_scl * ct2);
      auto v4 = mesh.add_vertex(x_scl * st2 * cp1, y_scl * st2 * sp1, z_scl * ct2);

      if (Q_UNLIKELY(t == 0)) {
        mesh.add_face(v1, v4, v3);
      }
      else if (Q_UNLIKELY(t + 1 == u_samples)) {
        mesh.add_face(v3, v2, v1);
      }
      else {
        mesh.add_face(v1, v4, v2);
        mesh.add_face(v2, v4, v3);
      }
    }
  }

  return mesh;
};

MeshFunction torus = [](const MeshParameters& params) {

  float R = params.at(OUTER_RADIUS).get<float>();
  float r = params.at(INNER_RADIUS).get<float>();
  int u_samples = params.at(U_SAMPLES).get<int>();
  int v_samples = params.at(V_SAMPLES).get<int>();

  Mesh mesh;
  auto f = [R, r](float su, float cu, float sv, float cv) {
    return vec3((R + r * cv) * cu, (R + r * cv) * su, r * sv);
  };

  for (int u = 0; u < u_samples; u++) {
    float u1 = 2 * M_PI * (static_cast<float>(u) / u_samples);
    float u2 = 2 * M_PI * (static_cast<float>(u + 1) / u_samples);

    float cu1 = qFastCos(u1), cu2 = qFastCos(u2);
    float su1 = qFastSin(u1), su2 = qFastSin(u2);

    for (int v = 0; v < v_samples; v++) {
      float v1 = 2 * M_PI * (static_cast<float>(v) / v_samples);
      float v2 = 2 * M_PI * (static_cast<float>(v + 1) / v_samples);

      float cv1 = qFastCos(v1), cv2 = qFastCos(v2);
      float sv1 = qFastSin(v1), sv2 = qFastSin(v2);
      auto i1 = mesh.add_vertex(f(su1, cu1, sv1, cv1));
      auto i2 = mesh.add_vertex(f(su2, cu2, sv1, cv1));
      auto i3 = mesh.add_vertex(f(su2, cu2, sv2, cv2));
      auto i4 = mesh.add_vertex(f(su1, cu1, sv2, cv2));

      mesh.add_face(i1, i2, i3);
      mesh.add_face(i3, i4, i1);
    }
  }

  return mesh;
};
}
}
}
