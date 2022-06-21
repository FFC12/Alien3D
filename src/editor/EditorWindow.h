#ifndef ALIEN3D_EDITOR_WINDOW_H
#define ALIEN3D_EDITOR_WINDOW_H
#include <Base.hpp>
#include <imgui.h>
#include <map>
#include <functional>

class EditorWindow {
private:
    static inline bool m_EditorOpen {true};

    static inline std::map<std::string, std::function<void()>> Components{};
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
        if(!ImGui::Begin("Alien3D Editor Window", &m_EditorOpen, ImGuiWindowFlags_None)){
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

#define COMPONENT_WIDGET_REGISTRY(name, callback) EditorWindow::ComponentWidgetRegistry(name,std::bind(&callback,this))
#endif