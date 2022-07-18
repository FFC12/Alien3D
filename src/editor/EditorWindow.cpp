#include <editor/EditorWindow.h>
#include <engine/GameObject.h>
#include <engine/Sprite.h>

void EditorWindow::DrawEditorWindow(float deltaTime, bool is2D, bool &gameMode) {
//        auto fps = 1 / deltaTime;
//        auto ms = deltaTime;
    if (!ImGui::Begin("Alien3D Editor", &EditorOpen, ImGuiWindowFlags_None | ImGuiWindowFlags_MenuBar)) {
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

    if (!GameMode) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.6f, 0.2f, 1.0f});
        if (ImGui::Button("Play")) {
            gameMode = true;
            GameMode = true;
        }
        ImGui::PopStyleColor();
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.6f, 0.2f, 0.2f, 1.0f});
        if (ImGui::Button("Stop")) {
            gameMode = false;
            GameMode = false;
        }
        ImGui::PopStyleColor();
    }

    //TODO: Add some delay...
    if (show) {
        if (!ImGui::Begin("Tools", &EditorOpen, ImGuiWindowFlags_None)) {
            ImGui::End();
            return;
        }

        if (!Widgets.empty()) {
            std::map<std::string, std::function<void()>>::iterator it;
            for (it = Widgets.begin(); it != Widgets.end(); it++) {
                it->second();
            }
        }

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.6f, 0.2f, 1.0f});
        if (ImGui::Button("Create GameObject"))
            ImGui::OpenPopup("Create GameObject");
        ImGui::PopStyleColor();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.5f, 0.5f, 0.5f, 1.0f});
        if (ImGui::BeginPopupModal("Create GameObject", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            static bool sprite = false;
            ImGui::Checkbox("Create as a sprite", &sprite);
            ImGui::Separator();

            static char buffer[128];

            ImGui::InputText("Name", &buffer[0], IM_ARRAYSIZE(buffer));
            auto n = strlen(buffer);
            std::string name(buffer, n);

            if (ImGui::Button("OK", ImVec2(120, 0))) {
                if (sprite) {
//                    SpriteCreateEvent(name);
                    Sprite::QuickCreate(name);
                } else {
//                    GameObjectCreateEvent(name);
                    GameObject::QuickCreate(name);
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor();

        ImGui::End();
    }

    ImGui::End();
}
