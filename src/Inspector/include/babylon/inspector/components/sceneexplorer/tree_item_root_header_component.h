#ifndef BABYLON_INSPECTOR_COMPONENTS_SCENE_EXPLORER_TREE_ITEM_ROOT_HEADER_COMPONENT_H
#define BABYLON_INSPECTOR_COMPONENTS_SCENE_EXPLORER_TREE_ITEM_ROOT_HEADER_COMPONENT_H

// ImGui
#include <imgui.h>

#include <babylon/babylon_api.h>
#include <babylon/imgui/icons_font_awesome_5.h>

namespace BABYLON {

struct BABYLON_SHARED_EXPORT TreeItemRootHeaderComponent {

  /** Font Awesome Icon Unicodes **/
  static constexpr const char* faBan = ICON_FA_BAN;

  static void render(const char* label)
  {
    // Arrow icon
    ImGui::TextWrapped("%s", faBan);
    ImGui::SameLine();
    // Text value
    ImGui::TextWrapped("%s", label);
  }

}; // end of struct TreeItemRootHeaderComponent

} // end of namespace BABYLON

#endif // end of
       // BABYLON_INSPECTOR_COMPONENTS_SCENE_EXPLORER_TREE_ITEM_ROOT_HEADER_COMPONENT_H