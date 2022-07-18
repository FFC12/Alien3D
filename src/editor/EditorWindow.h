#ifndef ALIEN3D_EDITOR_WINDOW_H
#define ALIEN3D_EDITOR_WINDOW_H

#include <Base.hpp>
#include <imgui.h>
#include <map>
#include <string>
#include <functional>

class EditorWindow {
private:
    static inline bool EditorOpen{true};
    static inline bool GameMode{false};

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

    static void DrawEditorWindow(float deltaTime, bool is2D, bool &gameMode);

    static inline std::function<void(const std::string &)> GameObjectCreateEvent{};
    static inline std::function<void(const std::string &)> SpriteCreateEvent{};
};

#define COMPONENT_WIDGET_REGISTRY(name, callback) EditorWindow::ComponentWidgetRegistry(name,std::bind(&callback,this))
#define COMPONENT_WIDGET_REGISTRY_S(name, callback) EditorWindow::ComponentWidgetRegistry(name,&callback)
#endif