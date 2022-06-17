#ifndef ALIEN3D_EDITOR_WINDOW_H
#define ALIEN3D_EDITOR_WINDOW_H
#include <Base.hpp>
#include <imgui.h>
#include <map>

class EditorWindow {
private:
    static inline bool m_EditorOpen {true};

    static std::map<std::string, std::function<void()>> Components;
public:
    EditorWindow() = default;

    static void ComponentWidgetRegistry(const std::string& compName, std::function<void()> compWidgetCallback) {
        if(Components.count(compName) > 0) {
            ALIEN_ERROR("Component has already registried!");
        } else {
            Components[compName] = compWidgetCallback;
        }
    }

    static inline void DrawEditorWindow() {
        if(!ImGui::Begin("Test Window", &m_EditorOpen, ImGuiWindowFlags_None)){
            ImGui::End();
            return;
        }

        if(!Components.empty()) {
            std::map<std::string, std::function<void()>>::iterator it;
            for(it = Components.begin(); it != Components.end(); it++) {
                it->second();
            }
        }

        ImGui::End();
    }

};

#endif