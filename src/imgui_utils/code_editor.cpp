#include <functional>
#include <string>
#include <iostream>
#include <babylon/core/filesystem.h>
#include <babylon/core/system.h>
#include <imgui_utils/icons_font_awesome_5.h>
#include <imgui_utils/imgui_utils.h>
#include <ImGuiColorTextEdit/TextEditor.h>
#include <imgui_utils/code_editor.h>
#include <imgui_utils/app_runner/imgui_runner.h>

namespace ImGuiUtils {

std::function<void(void)> guiAllowEdition;


class OneCodeEditor
{
public:
  OneCodeEditor(const std::string &filePath) : _filePath(filePath)
  {
    auto lang = TextEditor::LanguageDefinition::CPlusPlus();
    _textEditor.SetLanguageDefinition(lang);
    _fileContent_Startup = BABYLON::Filesystem::readFileContents(filePath.c_str());
    _fileContent_Saved = _fileContent_Startup;
    _textEditor.SetText(_fileContent_Startup);
  }

  void render()
  {
    //ImGui::ShowDemoWindow();
    checkExternalModifications();
    renderStatusLine();
    ImGui::Separator();
    renderCommandLine();
    ImGui::Separator();
    _textEditor.Render("TextEditor");
  }

  std::string filePath() const { return _filePath; }
  std::string baseName() const {
    return BABYLON::Filesystem::baseName(_filePath);
  }
  TextEditor & getTextEditor() { return _textEditor;  }

private:
  bool canSave()
  {
    if (_textEditor.IsReadOnly())
      return false;
    return (_textEditor.GetText() != _fileContent_Saved);
  }
  void save()
  {
    BABYLON::Filesystem::writeFileContents(_filePath.c_str(), _textEditor.GetText());
    _fileContent_Saved = _textEditor.GetText();
  }
  bool canRestore()
  {
    if (_textEditor.IsReadOnly())
      return false;
    return _textEditor.CanUndo();
  }
  void restore()
  {
    if (_textEditor.IsReadOnly())
      return;
    _textEditor.SetText(_fileContent_Startup);
  }

  void renderCommandLine()
  {
    if (!_textEditor.IsReadOnly())
    {
      if (ImGuiUtils::Button_WithEnable(ICON_FA_SAVE " Save", canSave()))
      {
        save();
      }
      ImGui::SameLine();
      if (ImGuiUtils::Button_WithEnable(ICON_FA_UNDO ICON_FA_UNDO " Restore", canRestore()))
        restore();
      ImGui::SameLine(0.f, 50.f);

      if (ImGuiUtils::Button_WithEnable(ICON_FA_UNDO, _textEditor.CanUndo()))
        _textEditor.Undo();
      ImGui::SameLine();

      if (ImGuiUtils::Button_WithEnable(ICON_FA_REDO, _textEditor.CanRedo()))
        _textEditor.Redo();
      ImGui::SameLine();

      if (ImGuiUtils::Button_WithEnable(ICON_FA_COPY, _textEditor.HasSelection()))
        _textEditor.Copy();
      ImGui::SameLine();
      if (ImGuiUtils::Button_WithEnable(ICON_FA_CUT, _textEditor.HasSelection()))
        _textEditor.Cut();
      ImGui::SameLine();
      if (ImGuiUtils::Button_WithEnable(ICON_FA_PASTE, ImGui::GetClipboardText() != nullptr))
        _textEditor.Paste();
    }

  }

  void renderStatusLine()
  {

    auto cpos = _textEditor.GetCursorPosition();
    bool isTextModified = (_fileContent_Saved != _textEditor.GetText());
    ImGui::Text("%s", _filePath.c_str());
    ImGui::Text("%6d/%-6d %6d lines | %s | %s | %s ", cpos.mLine + 1, cpos.mColumn + 1, _textEditor.GetTotalLines(),
      isTextModified ? "*" : " ",
      _textEditor.GetLanguageDefinition().mName.c_str(),
      _textEditor.IsOverwrite() ? "Ovr" : "Ins"
      );

    ImGui::SameLine(0., 100.f);
    if (guiAllowEdition)
      guiAllowEdition();
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_EDIT "external editor"))
      BABYLON::System::openFile(_filePath);
  }

  void checkExternalModifications()
  {
    static int counter = 0;
    counter++;
    // only check every 2 seconds
    if (counter % (50 * 2) == 0)
    {
      std::string newContent = BABYLON::Filesystem::readFileContents(_filePath.c_str());
      bool wasExternallyModified = (newContent != _fileContent_Saved);
      bool wasInternallyModified = (_fileContent_Saved != _textEditor.GetText());
      if (wasExternallyModified && (!wasInternallyModified))
      {
        _textEditor.SetText(newContent);
      }
      if (wasExternallyModified && wasInternallyModified)
      {
        std::cerr << "File was modified externally and internally!";
      }
    }
  }

  std::string _filePath;
  TextEditor _textEditor;
  std::string _fileContent_Startup;
  std::string _fileContent_Saved;
};



class MultipleCodeEditorImpl
{
public:
  MultipleCodeEditorImpl(bool showCheckboxReadOnly) 
    : _showCheckboxReadOnly(showCheckboxReadOnly)
  {
    if (!_showCheckboxReadOnly)
      _canEdit = true;
  }

  void setFiles(const std::vector<std::string> &filePaths)
  {
    _editors.clear();
    for (auto filePath : filePaths)
      _editors.push_back(OneCodeEditor(filePath));
    if (!_editors.empty())
      _currentEditor = &_editors.back();

    guiAllowEdition = [this]() {
      this->renderAllowEdition();
    };

    updateReadOnly();
  }

  void updateReadOnly()
  {
    for (auto & editor : _editors)
    {
      editor.getTextEditor().SetReadOnly(!_canEdit);
    }
  }

  void renderAllowEdition()
  {
    if (_showCheckboxReadOnly)
    {
      if (ImGui::Checkbox("Allow edition", &_canEdit))
        updateReadOnly();
    }
  }

  void render()
  {
    if (_editors.empty())
      return;

    renderTabs();
    ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 200.f);
    renderPalette();
    if (_currentEditor)
      currentEditor().render();
  }

private:
  void renderTabs()
  {
    if (_editors.size() < 2)
      return;
    for (auto & editor : _editors)
    {
      bool isCurrent = (_currentEditor == &editor);
      if (isCurrent)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.6f, 1.f));
      if (ImGui::Button(editor.baseName().c_str()))
      {
        _currentEditor = &editor;
      }
      if (isCurrent)
        ImGui::PopStyleColor();
      ImGui::SameLine();
    }
    ImGui::NewLine();
  }

  void renderPalette()
  {
    //ImGui::Text("Palette"); ImGui::SameLine();
    if (ImGui::RadioButton("Dark", &_palette, 0))
      for (auto & editor : _editors)
        editor.getTextEditor().SetPalette(TextEditor::GetDarkPalette());
    ImGui::SameLine();
    if (ImGui::RadioButton("Light", &_palette, 1))
      for (auto & editor : _editors)
        editor.getTextEditor().SetPalette(TextEditor::GetLightPalette());
    ImGui::SameLine();
    if (ImGui::RadioButton("Blue", &_palette, 2))
      for (auto & editor : _editors)
        editor.getTextEditor().SetPalette(TextEditor::GetRetroBluePalette());
  }


  OneCodeEditor & currentEditor()
  {
    return *_currentEditor;
  }

  std::vector<OneCodeEditor> _editors;
  OneCodeEditor * _currentEditor = nullptr;
  int _palette = 0;
  bool _canEdit = false;
  bool _showCheckboxReadOnly;
};




CodeEditor::CodeEditor(bool showCheckboxReadOnly)
  : _pImpl(std::make_unique<MultipleCodeEditorImpl>(showCheckboxReadOnly))
{
}

void CodeEditor::setFiles(const std::vector<std::string> &filePaths)
{
  _pImpl->setFiles(filePaths);
}

CodeEditor::~CodeEditor() = default;

void CodeEditor::render()
{
  return _pImpl->render();
}


void demoCodeEditor()
{
  std::vector<std::string> path =
  {
    "../../../src/imgui_babylon/src/imgui_babylon/sample_list_page.cpp",
    "../../../src/imgui_babylon/include/babylon/imgui_babylon/sample_list_page.h"
  };
  ImGuiUtils::CodeEditor codeEditor(true);
  codeEditor.setFiles(path);
  auto demoGui = [&]() {
    codeEditor.render();
  };

  ImGuiUtils::ImGuiRunner::AppWindowParams params;
  //params.FullScreen = true;
  params.Title = "Hello World";
  params.ShowMenuBar = true;
  ImGuiUtils::ImGuiRunner::RunGui(demoGui, params);

}

} // namespace ImGuiUtils
