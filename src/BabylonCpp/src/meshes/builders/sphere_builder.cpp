#include <babylon/meshes/builders/sphere_builder.h>

#include <babylon/meshes/mesh.h>
#include <babylon/meshes/vertex_data.h>
#include <babylon/meshes/vertex_data_options.h>

namespace BABYLON {

MeshPtr SphereBuilder::CreateSphere(const std::string& name,
                                    SphereOptions& options, Scene* scene)
{
  auto sphere = Mesh::New(name, scene);

  options.sideOrientation
    = Mesh::_GetDefaultSideOrientation(options.sideOrientation);
  sphere->_originalBuilderSideOrientation = options.sideOrientation;

  auto vertexData = VertexData::CreateSphere(options);

  vertexData->applyToMesh(*sphere, options.updatable);

  return sphere;
}

} // end of namespace BABYLON