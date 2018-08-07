#ifndef BABYLON_LIGHTS_POINT_LIGHT_H
#define BABYLON_LIGHTS_POINT_LIGHT_H

#include <babylon/babylon_global.h>
#include <babylon/lights/shadow_light.h>

namespace BABYLON {

/**
 * @brief A point light is a light defined by an unique point in world space.
 * The light is emitted in every direction from this point.
 * A good example of a point light is a standard light bulb.
 * Documentation: https://doc.babylonjs.com/babylon101/lights
 */
class BABYLON_SHARED_EXPORT PointLight : public ShadowLight {

public:
  template <typename... Ts>
  static PointLight* New(Ts&&... args)
  {
    auto light = new PointLight(::std::forward<Ts>(args)...);
    light->addToScene(static_cast<unique_ptr_t<Light>>(light));
    if (!PointLight::NodeConstructorAdded && PointLight::AddNodeConstructor) {
      PointLight::AddNodeConstructor();
    }

    return light;
  }
  ~PointLight() override;

  IReflect::Type type() const override;

  /**
   * @brief Getter: In case of direction provided, the shadow will not use a
   * cube texture but simulate a spot shadow as a fallback
   * This specifies what angle the shadow will use to be created.
   *
   * It default to 90 degrees to work nicely with the cube texture generation
   * for point lights shadow maps.
   */
  float shadowAngle() const;

  /**
   * @brief Setter: In case of direction provided, the shadow will not use a
   * cube texture but simulate a spot shadow as a fallback
   * This specifies what angle the shadow will use to be created.
   *
   * It default to 90 degrees to work nicely with the cube texture generation
   * for point lights shadow maps.
   */
  void setShadowAngle(float value);

  /**
   * @brief Gets the direction if it has been set.
   * In case of direction provided, the shadow will not use a cube texture but
   * simulate a spot shadow as a fallback
   */
  Vector3& direction() override;

  /**
   * @brief In case of direction provided, the shadow will not use a cube
   * texture but simulate a spot shadow as a fallback
   */
  void setDirection(const Vector3& value);

  /**
   * @brief Returns the string "PointLight"
   * @returns the class name
   */
  const string_t getClassName() const override;

  /**
   * @brief Returns the integer 0.
   * @returns The light Type id as a constant defines in Light.LIGHTTYPEID_x
   */
  unsigned int getTypeID() const override;

  /**
   * @brief Specifies wether or not the shadowmap should be a cube texture.
   * @returns true if the shadowmap needs to be a cube texture.
   */
  bool needCube() const override;

  /**
   * @brief Returns a new Vector3 aligned with the PointLight cube system
   * according to the passed cube face index (integer).
   * @param faceIndex The index of the face we are computed the direction to
   * generate shadow
   * @returns The set direction in 2d mode otherwise the direction to the
   * cubemap face if needCube() is true
   */
  Vector3 getShadowDirection(unsigned int faceIndex) override;

  /**
   * @brief Sets the passed Effect "effect" with the PointLight transformed
   * position (or position, if none) and passed name (string).
   * @param effect The effect to update
   * @param lightIndex The index of the light in the effect to update
   * @returns The point light
   */
  void transferToEffect(Effect* effect, const string_t& lightIndex) override;

  /**
   * @brief Prepares the list of defines specific to the light type.
   * @param defines the list of defines
   * @param lightIndex defines the index of the light for the effect
   */
  void prepareLightSpecificDefines(MaterialDefines& defines,
                                   unsigned int lightIndex) override;

protected:
  /**
   * @brief Creates a PointLight object from the passed name and position
   * (Vector3) and adds it in the scene. A PointLight emits the light in every
   * direction. It can cast shadows. If the scene camera is already defined and
   * you want to set your PointLight at the camera position, just set it :
   * ```javascript
   * var pointLight = new BABYLON.PointLight("pl", camera.position, scene);
   * ```
   * Documentation : http://doc.babylonjs.com/tutorials/lights
   * @param name The light friendly name
   * @param position The position of the point light in the scene
   * @param scene The scene the lights belongs to
   */
  PointLight(const string_t& name, const Vector3& position, Scene* scene);

  /**
   * @brief Sets the passed matrix "matrix" as a left-handed perspective
   * projection matrix with the following settings:
   * - fov = PI / 2
   * - aspect ratio : 1.0
   * - z-near and far equal to the active camera minZ and maxZ.
   * Returns the PointLight.
   */
  void _setDefaultShadowProjectionMatrix(
    Matrix& matrix, const Matrix& viewMatrix,
    const vector_t<AbstractMesh*>& renderList) override;

  void _buildUniformLayout() override;

private:
  float _shadowAngle;

  static bool NodeConstructorAdded;
  static ::std::function<void()> AddNodeConstructor;

}; // end of class PointLight

} // end of namespace BABYLON

#endif // end of BABYLON_LIGHTS_POINT_LIGHT_H
