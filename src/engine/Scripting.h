#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <editor/EditorWindow.h>
#include <api/PythonBridge.h>

class Scripting : public Component {
public:
    Scripting() {
        componentType = GAME_SCRIPT;
    }

    explicit Scripting(const std::string &path) {
        componentType = GAME_SCRIPT;
        m_ScriptPath = path;
        loadScriptFromFile(path);
    }

    void loadScriptFromFile(const std::string &path) {
        auto &pythonAPI = PythonBridge::getInstance();
        pythonAPI.loadPythonScriptFromFile(path);
    }

    static void init() {
        PythonBridge::getInstance();
        PythonBridge::getInstance().exec();
        PythonBridge::getInstance().onInit();
    }

    static void run() {
        PythonBridge::getInstance().onUpdate();
    }

    void OnComponentWidgetDrawn() override {
        this->scriptingWidget();
    }

private:
    void scriptingWidget() {
        if (ImGui::TreeNode("Scripting")) {
            ImGui::BulletText("Script Path: %s", m_ScriptPath.c_str());
            ImGui::TreePop();
        }

        ImGui::Separator();
    }

    std::string m_ScriptPath;
};

#endif