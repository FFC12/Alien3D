#ifndef ALIEN3D_EDITOR_WINDOW_H
#define ALIEN3D_EDITOR_WINDOW_H

#include <Base.hpp>
#include <imgui.h>
#include <map>
#include <functional>

class EditorWindow {
private:
    static inline bool m_EditorOpen{true};

    static inline std::map<std::string, std::function<void()>> Widgets{};
public:
    EditorWindow() = default;

    static void ComponentWidgetRegistry(const std::string &compName, std::function<void()> compWidgetCallback) {
        if (Widgets.count(compName) > 0) {
            ALIEN_ERROR("Component has already registried!");
        } else {
            Widgets[compName] = compWidgetCallback;
        }
    }

    static inline void DrawEditorWindow() {
        if (!ImGui::Begin("Alien3D Editor Window", &m_EditorOpen, ImGuiWindowFlags_None)) {
            ImGui::End();
            return;
        }

        if (!Widgets.empty()) {
            std::map<std::string, std::function<void()>>::iterator it;
            for (it = Widgets.begin(); it != Widgets.end(); it++) {
                it->second();
            }
        }

        ImGui::End();
    }

};

#define COMPONENT_WIDGET_REGISTRY(name, callback) EditorWindow::ComponentWidgetRegistry(name,std::bind(&callback,this))
#define COMPONENT_WIDGET_REGISTRY_S(name, callback) EditorWindow::ComponentWidgetRegistry(name,&callback)
#endif