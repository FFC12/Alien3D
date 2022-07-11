#ifndef ALIEN3D_EDITOR_WINDOW_H
#define ALIEN3D_EDITOR_WINDOW_H

#include <Base.hpp>
#include <imgui.h>
#include <map>
#include <functional>
#include "api/LuaBridge.hpp"

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

    static inline void DrawEditorWindow(float deltaTime, bool is2D) {
//        auto fps = 1 / deltaTime;
//        auto ms = deltaTime;
        if (!ImGui::Begin("Alien3D Editor", &m_EditorOpen, ImGuiWindowFlags_None | ImGuiWindowFlags_MenuBar)) {
            ImGui::End();
            return;
        }

        static bool show = false;
        static bool saveGame = false;
        static bool buildGame = false;
        static bool settings = false;

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Menu")) {
                ImGui::MenuItem("Save", NULL, &saveGame);
                ImGui::MenuItem("Build Game", NULL, &buildGame);
                ImGui::MenuItem("Settings", NULL, &settings);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Editor Tools")) {
                ImGui::MenuItem("GameObject List", NULL, &show);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Text("FPS: %.2f \n ms: %.3f", 1 / deltaTime, deltaTime);
        std::string mode = is2D ? "2D Mode" : "3D Mode";
        ImGui::Text("%s", mode.c_str());

        //TODO: Add some delay...
        if (show) {
            if (!ImGui::Begin("Tools", &m_EditorOpen, ImGuiWindowFlags_None)) {
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

        ImGui::End();
    }

};

#define COMPONENT_WIDGET_REGISTRY(name, callback) EditorWindow::ComponentWidgetRegistry(name,std::bind(&callback,this))
#define COMPONENT_WIDGET_REGISTRY_S(name, callback) EditorWindow::ComponentWidgetRegistry(name,&callback)
#endif