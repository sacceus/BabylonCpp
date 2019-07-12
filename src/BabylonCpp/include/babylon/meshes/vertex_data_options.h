#ifndef BABYLON_MESHES_VERTEX_DATA_OPTIONS_H
#define BABYLON_MESHES_VERTEX_DATA_OPTIONS_H

#include <functional>

#include <babylon/babylon_api.h>
#include <babylon/core/structs.h>
#include <babylon/culling/bounding_info.h>
#include <babylon/math/color3.h>
#include <babylon/math/color4.h>
#include <babylon/math/isize.h>
#include <babylon/math/vector4.h>

namespace BABYLON {

class GroundMesh;
class LinesMesh;
class Mesh;
using LinesMeshPtr = std::shared_ptr<LinesMesh>;
using MeshPtr      = std::shared_ptr<Mesh>;

//------------------------------------------------------------------------------
// Dashed lines mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a dashed lines mesh.
 */
class BABYLON_SHARED_EXPORT DashedLinesOptions {

public:
  DashedLinesOptions();
  ~DashedLinesOptions();

public:
  float dashSize;
  float gapSize;
  unsigned int dashNb;
  std::vector<Vector3> points;
  bool updatable;
  LinesMeshPtr instance;

}; // end of class DashedLinesOptions

//------------------------------------------------------------------------------
// Extrude shape mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create an extruded shape mesh.
 */
class BABYLON_SHARED_EXPORT ExtrudeShapeOptions {

public:
  /** Statics **/
  static Vector4 DefaultFrontUVs;
  static Vector4 DefaultBackUVs;

public:
  ExtrudeShapeOptions();
  ~ExtrudeShapeOptions();

public:
  std::vector<Vector3> shape;
  std::vector<Vector3> path;
  float rotation;
  float scale;
  unsigned int cap;
  MeshPtr instance;
  bool invertUV;
  unsigned int sideOrientation;
  Vector4 frontUVs;
  Vector4 backUVs;
  bool updatable;

}; // end of class ExtrudeShapeOptions

//------------------------------------------------------------------------------
// Extrude shape custom mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create an custom extruded shape mesh.
 */
class BABYLON_SHARED_EXPORT ExtrudeShapeCustomOptions {

public:
  /** Statics **/
  static Vector4 DefaultFrontUVs;
  static Vector4 DefaultBackUVs;

public:
  ExtrudeShapeCustomOptions();
  ~ExtrudeShapeCustomOptions();

public:
  std::vector<Vector3> shape;
  std::vector<Vector3> path;
  std::function<float(float i, float distance)> scaleFunction;
  std::function<float(float i, float distance)> rotationFunction;
  bool ribbonCloseArray;
  bool ribbonClosePath;
  unsigned int cap;
  MeshPtr instance;
  bool invertUV;
  unsigned int sideOrientation;
  Vector4 frontUVs;
  Vector4 backUVs;
  bool updatable;

}; // end of class ExtrudeShapeCustomOptions

//------------------------------------------------------------------------------
// Ground from height map mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a ground from height map mesh.
 */
class BABYLON_SHARED_EXPORT GroundFromHeightMapOptions {

public:
  GroundFromHeightMapOptions();
  ~GroundFromHeightMapOptions();

public:
  float width;
  float height;
  unsigned int subdivisions;
  float minHeight;
  float maxHeight;
  Uint8Array buffer;
  unsigned int bufferWidth;
  unsigned int bufferHeight;
  std::optional<float> alphaFilter;
  Color3 colorFilter;
  bool updatable;
  std::function<void(GroundMesh* mesh)> onReady;

}; // end of class GroundFromHeightMapOptions

//------------------------------------------------------------------------------
// Ground mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a ground mesh.
 */
class BABYLON_SHARED_EXPORT GroundOptions {

public:
  GroundOptions(unsigned int subdivisions = 1);
  ~GroundOptions();

public:
  unsigned int width;
  unsigned int height;
  unsigned int subdivisionsX;
  unsigned int subdivisionsY;
  bool updatable;

}; // end of class GroundOptions

//------------------------------------------------------------------------------
// Lines mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a lines mesh.
 */
class BABYLON_SHARED_EXPORT LinesOptions {

public:
  LinesOptions();
  ~LinesOptions();

public:
  std::vector<Vector3> points;
  std::vector<Color4> colors;
  bool updatable;
  bool useVertexAlpha;
  LinesMeshPtr instance;

}; // end of class LinesOptions

//------------------------------------------------------------------------------
// Line system mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a line system mesh.
 */
class BABYLON_SHARED_EXPORT LineSystemOptions {

public:
  LineSystemOptions();
  LineSystemOptions(LinesOptions& linesOptions);
  ~LineSystemOptions();

public:
  std::vector<std::vector<Vector3>> lines;
  std::vector<std::vector<Color4>> colors;
  bool updatable;
  bool useVertexAlpha;
  LinesMeshPtr instance;

}; // end of class LineSystemOptions

//------------------------------------------------------------------------------
// Plane mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a plane mesh.
 */
class BABYLON_SHARED_EXPORT PlaneOptions {

public:
  /** Statics **/
  static Vector4 DefaultFrontUVs;
  static Vector4 DefaultBackUVs;

public:
  PlaneOptions();
  PlaneOptions(float size);
  ~PlaneOptions();

public:
  float width;
  float height;
  unsigned int sideOrientation;
  Vector4 frontUVs;
  Vector4 backUVs;
  bool updatable;
  Plane* sourcePlane;

}; // end of class PlaneOptions

//------------------------------------------------------------------------------
// Polygon mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a polygon mesh.
 */
class BABYLON_SHARED_EXPORT PolygonOptions {

public:
  /** Statics **/
  static Vector4 DefaultFrontUVs;
  static Vector4 DefaultBackUVs;

public:
  PolygonOptions();
  ~PolygonOptions();

public:
  std::vector<Vector3> shape;
  std::vector<std::vector<Vector3>> holes;
  float depth;
  std::vector<Vector4> faceUV;
  std::vector<Color4> faceColors;
  bool updatable;
  unsigned int sideOrientation;
  Vector4 frontUVs;
  Vector4 backUVs;

}; // end of class PolygonOptions

//------------------------------------------------------------------------------
// Polyhedron mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a polyhedron mesh.
 */
class BABYLON_SHARED_EXPORT PolyhedronOptions {

public:
  /** Statics **/
  static Vector4 DefaultFrontUVs;
  static Vector4 DefaultBackUVs;

public:
  PolyhedronOptions();
  PolyhedronOptions(float size);
  ~PolyhedronOptions();

public:
  unsigned int type;
  float size;
  float sizeX;
  float sizeY;
  float sizeZ;
  std::vector<Color4> faceColors;
  std::vector<Vector4> faceUV;
  Polyhedron custom;
  bool flat;
  unsigned int sideOrientation;
  Vector4 frontUVs;
  Vector4 backUVs;
  bool updatable;

}; // end of class PolyhedronOptions

//------------------------------------------------------------------------------
// Tiled ground mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a tiled ground mesh.
 */
class BABYLON_SHARED_EXPORT TiledGroundOptions {

public:
  TiledGroundOptions();
  ~TiledGroundOptions();

public:
  float xmin, zmin;
  float xmax, zmax;
  ISize subdivisions;
  ISize precision;
  bool updatable;

}; // end of class TiledGroundOptions

//------------------------------------------------------------------------------
// TorusKnot mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a torus knot mesh.
 */
class BABYLON_SHARED_EXPORT TorusKnotOptions {

public:
  /** Statics **/
  static Vector4 DefaultFrontUVs;
  static Vector4 DefaultBackUVs;

public:
  TorusKnotOptions();
  ~TorusKnotOptions();

public:
  float radius;
  float tube;
  unsigned int radialSegments;
  unsigned int tubularSegments;
  float p;
  float q;
  unsigned int sideOrientation;
  Vector4 frontUVs;
  Vector4 backUVs;
  bool updatable;

}; // end of class TorusKnotOptions

//------------------------------------------------------------------------------
// Torus mesh options
//------------------------------------------------------------------------------

/**
 * @brief Options used to create a torus mesh.
 */
class BABYLON_SHARED_EXPORT TorusOptions {

public:
  /** Statics **/
  static Vector4 DefaultFrontUVs;
  static Vector4 DefaultBackUVs;

public:
  TorusOptions();
  ~TorusOptions();

public:
  float diameter;
  float thickness;
  unsigned int tessellation;
  unsigned int sideOrientation;
  Vector4 frontUVs;
  Vector4 backUVs;
  bool updatable;

}; // end of class TorusOptions

} // end of namespace BABYLON

#endif // end of BABYLON_MESHES_VERTEX_DATA_OPTIONS_H
