#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <editor/EditorWindow.h>
#include <api/PythonBridge.h>

class Scripting : public Component {
public:
    Scripting() = default;

    explicit Scripting(const std::string &path) {
        m_ScriptPath = path;
        loadScriptFromFile(path);
    }

    void loadScriptFromFile(const std::string &path) {
        auto &pythonAPI = PythonBridge::getInstance();
        pythonAPI.loadPythonScriptFromFile(path);
    }

    static void init() {
        PythonBridge::getInstance();
    }

    static void run() {
        PythonBridge::getInstance().exec();
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