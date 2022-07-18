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
        auto script = loadScriptFromFile(path);
        exec(script);
    }

    void setPath(const std::string& path) {
        this->m_ScriptPath = path;
    }

    void exec(const std::string &script) {
        auto pos = script.find_last_of('.');
        auto scriptName = script.substr(0, pos);
        auto modulePath = "res.scripts." + scriptName;

        ALIEN_INFO("Executing script '" + scriptName + "'");

        auto scriptModule = py::module_::import(modulePath.c_str());
        py::type scriptClass = scriptModule.attr("GameScript");

        m_PythonObject = new py::object(scriptClass());

        Functions[m_UUID] = {
                [&]() { onInit(); },
                [&]() { onUpdate(); }
        };
    }

    std::string loadScriptFromFile(const std::string &path) {
        auto data = Utils::ReadFile(path);

        auto pos = path.find_last_of('/');
        auto scriptName = path.substr(pos + 1, path.size());

        m_ScriptData = std::move(data);
        return scriptName;
    }

    void onInit() {
        PythonBridge::getInstance();
        PythonBridge::getInstance().execInit(*m_PythonObject);
    }

    void onUpdate() {
        PythonBridge::getInstance().execUpdate(*m_PythonObject);
    }

    void OnComponentWidgetDrawn() override {
        this->scriptingWidget();
    }

    static inline std::map<std::string, std::pair<std::function<void()>, std::function<void()>>> Functions{};
private:
    void scriptingWidget() {
        if (ImGui::TreeNode("Scripting")) {
            ImGui::BulletText("Script Path: %s", m_ScriptPath.c_str());

            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly;

            ImGui::InputTextMultiline("##source", const_cast<char *>(m_ScriptData.c_str()), m_ScriptData.size(),
                                      ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);

            ImGui::TreePop();
        }
        ImGui::Separator();
    }

    std::string m_ScriptPath;
    std::string m_ScriptData;
    py::object *m_PythonObject;
};

#endif