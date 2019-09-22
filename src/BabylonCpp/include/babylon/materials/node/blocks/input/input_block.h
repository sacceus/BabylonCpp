#ifndef BABYLON_MATERIALS_NODE_BLOCKS_INPUT_INPUT_BLOCK_H
#define BABYLON_MATERIALS_NODE_BLOCKS_INPUT_INPUT_BLOCK_H

#include <functional>

#include <babylon/babylon_api.h>
#include <babylon/materials/node/blocks/input/animated_input_block_types.h>
#include <babylon/materials/node/node_material_block.h>
#include <babylon/materials/node/node_material_block_connection_point_mode.h>
#include <babylon/materials/node/node_material_system_values.h>

namespace BABYLON {

class InputBlock;
class InputValue;
class Matrix;
using InputBlockPtr = std::shared_ptr<InputBlock>;
using InputValuePtr = std::shared_ptr<InputValue>;

/**
 * @brief Block used to expose an input value.
 */
class BABYLON_SHARED_EXPORT InputBlock : public NodeMaterialBlock {

public:
  template <typename... Ts>
  static InputBlockPtr New(Ts&&... args)
  {
    return std::shared_ptr<InputBlock>(
      new InputBlock(std::forward<Ts>(args)...));
  }
  ~InputBlock() override;

  /**
   * @brief Set the source of this connection point to a vertex attribute.
   * @param attributeName defines the attribute name (position, uv, normal,
   * etc...). If not specified it will take the connection point name
   * @returns the current connection point
   */
  InputBlock& setAsAttribute(const std::string& attributeName = "");

  /**
   * @brief Set the source of this connection point to a system value.
   * @param value define the system value to use (world, view, etc...) or null
   * to switch to manual value
   * @returns the current connection point
   */
  InputBlock&
  setAsSystemValue(const std::optional<NodeMaterialSystemValues>& value);

  /**
   * @brief Gets the current class name.
   * @returns the class name
   */
  const std::string getClassName() const override;

  /**
   * @brief Animate the input if animationType !== None
   * @param scene defines the rendering scene
   */
  void animate(Scene* scene);

  /**
   * @brief Set the input block to its default value (based on its type).
   */
  void setDefaultValue();

  /**
   * @brief Hidden.
   */
  void _transmitWorld(Effect* effect, const Matrix& world,
                      const Matrix& worldView, Matrix& worldViewProjection);

  /**
   * @brief Hidden.
   */
  void _transmit(Effect* effect, Scene* scene);

  /**
   * @brief Serializes this block in a JSON representation.
   * @returns the serialized block object
   */
  json serialize() const override;

  /**
   * @brief Hidden
   */
  void _deserialize(const json& serializationObject, Scene* scene,
                    const std::string& rootUrl) override;

protected:
  /**
   * @brief Creates a new InputBlock.
   * @param name defines the block name
   * @param target defines the target of that block (Vertex by default)
   * @param type defines the type of the input (can be set to
   * NodeMaterialBlockConnectionPointTypes.AutoDetect)
   */
  InputBlock(const std::string& name,
             NodeMaterialBlockTargets target = NodeMaterialBlockTargets::Vertex,
             NodeMaterialBlockConnectionPointTypes type
             = NodeMaterialBlockConnectionPointTypes::AutoDetect);

  /**
   * @brief Gets or sets the connection point type (default is float).
   */
  NodeMaterialBlockConnectionPointTypes& get_type();

  /**
   * @brief Gets the output component.
   */
  NodeMaterialConnectionPointPtr& get_output();

  /**
   * @brief Gets the value of that point.
   * Please note that this value will be ignored if valueCallback is defined
   */
  InputValuePtr& get_value();

  /**
   * @brief Sets the value of that point.
   * Please note that this value will be ignored if valueCallback is defined
   */
  void set_value(const InputValuePtr& value);

  /**
   * @brief Gets a callback used to get the value of that point.
   * Please note that setting this value will force the connection point to
   * ignore the value property
   */
  std::function<InputValuePtr()>& get_valueCallback();

  /**
   * @brief Sets a callback used to get the value of that point.
   * Please note that setting this value will force the connection point to
   * ignore the value property
   */
  void set_valueCallback(const std::function<InputValuePtr()>& value);

  /**
   * @brief Gets the associated variable name in the shader.
   */
  std::string get_associatedVariableName() const;

  /**
   * @brief Sets the associated variable name in the shader.
   */
  void set_associatedVariableName(std::string value);

  /**
   * @brief Gets the type of animation applied to the input.
   */
  AnimatedInputBlockTypes& get_animationType();

  /**
   * @brief Sets the type of animation applied to the input.
   */
  void set_animationType(const AnimatedInputBlockTypes& value);

  /**
   * @brief Gets a boolean indicating that this connection point not defined
   * yet.
   */
  bool get_isUndefined() const;

  /**
   * @brief Gets a boolean indicating that this connection point is coming from
   * an uniform. In this case the connection point name must be the name of the
   * uniform to use. Can only be set on inputs.
   */
  bool get_isUniform() const;

  /**
   * @brief Sets a boolean indicating that this connection point is coming from
   * an uniform. In this case the connection point name must be the name of the
   * uniform to use. Can only be set on inputs.
   */
  void set_isUniform(bool value);

  /**
   * @brief Gets a boolean indicating that this connection point is coming from
   * an attribute. In this case the connection point name must be the name of
   * the attribute to use Can only be set on inputs.
   */
  bool get_isAttribute() const;

  /**
   * @brief Sets a boolean indicating that this connection point is coming from
   * an attribute. In this case the connection point name must be the name of
   * the attribute to use Can only be set on inputs.
   */
  void set_isAttribute(bool value);

  /**
   * @brief Gets a boolean indicating that this connection point is generating a
   * varying variable. Can only be set on exit points.
   */
  bool get_isVarying() const;

  /**
   * @brief Sets a boolean indicating that this connection point is generating a
   * varying variable. Can only be set on exit points.
   */
  void set_isVarying(bool value);

  /**
   * @brief Gets a boolean indicating that the current connection point is a
   * system value.
   */
  bool get_isSystemValue() const;

  /**
   * @brief Gets the current well known value or null if not defined as a system
   * value.
   */
  std::optional<NodeMaterialSystemValues>& get_systemValue();

  /**
   * @brief sets the current well known value or null if not defined as a system
   * value.
   */
  void set_systemValue(const std::optional<NodeMaterialSystemValues>& value);

  /**
   * @brief Hidden
   */
  std::string _dumpPropertiesCode() override;

  /**
   * @brief Hidden
   */
  InputBlock& _buildBlock(NodeMaterialBuildState& state) override;

private:
  std::string _emitDefine(const std::string& define) const;
  void _emit(NodeMaterialBuildState& state, const std::string& define = "");

public:
  /**
   * Hidden
   */
  std::optional<NodeMaterialSystemValues> _systemValue;

  /**
   * Gets or sets a boolean indicating that this input can be edited in the
   * Inspector (false by default)
   */
  bool visibleInInspector;

  /**
   * Gets or sets the connection point type (default is float)
   */
  ReadOnlyProperty<InputBlock, NodeMaterialBlockConnectionPointTypes> type;

  /**
   * Gets the output component
   */
  ReadOnlyProperty<InputBlock, NodeMaterialConnectionPointPtr> output;

  /**
   * Gets or sets the value of that point.
   * Please note that this value will be ignored if valueCallback is defined
   */
  Property<InputBlock, InputValuePtr> value;

  /**
   * Gets or sets a callback used to get the value of that point.
   * Please note that setting this value will force the connection point to
   * ignore the value property
   */
  Property<InputBlock, std::function<InputValuePtr()>> valueCallback;

  /**
   * Gets or sets the associated variable name in the shader
   */
  Property<InputBlock, std::string> associatedVariableName;

  /**
   * Gets or sets the type of animation applied to the input
   */
  Property<InputBlock, AnimatedInputBlockTypes> animationType;

  /**
   * Gets a boolean indicating that this connection point not defined yet
   */
  ReadOnlyProperty<InputBlock, bool> isUndefined;

  /**
   * Gets or sets a boolean indicating that this connection point is coming from
   * an uniform. In this case the connection point name must be the name of the
   * uniform to use. Can only be set on inputs
   */
  Property<InputBlock, bool> isUniform;

  /**
   * Gets or sets a boolean indicating that this connection point is coming from
   * an attribute. In this case the connection point name must be the name of
   * the attribute to use Can only be set on inputs
   */
  Property<InputBlock, bool> isAttribute;

  /**
   * Gets or sets a boolean indicating that this connection point is generating
   * a varying variable. Can only be set on exit points
   */
  Property<InputBlock, bool> isVarying;

  /**
   * Gets a boolean indicating that the current connection point is a system
   * value
   */
  ReadOnlyProperty<InputBlock, bool> isSystemValue;

  /**
   * Gets or sets the current well known value or null if not defined as a
   * system value
   */
  Property<InputBlock, std::optional<NodeMaterialSystemValues>> systemValue;

private:
  NodeMaterialBlockConnectionPointMode _mode;
  std::string _associatedVariableName;
  InputValuePtr _storedValue;
  std::function<InputValuePtr()> _valueCallback;
  NodeMaterialBlockConnectionPointTypes _type;
  AnimatedInputBlockTypes _animationType;

}; // end of class InputBlock

} // end of namespace BABYLON

#endif // end of BABYLON_MATERIALS_NODE_BLOCKS_INPUT_INPUT_BLOCK_H
