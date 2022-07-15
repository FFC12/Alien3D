#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <gfx/GfxBase.h>
#include <gfx/WorldCamera.h>
#include <engine/RenderQueue.h>
#include <engine/Physics2D.h>

#ifdef LIB_GLFW

#include <GLFW/glfw3.h>
#include <adapter/GLFWImguiAdapter.h>
#include <editor/EditorWindow.h>

#endif


// TODO: Set callback for detecting when window resizes
class AlienApplication {
public:
    explicit AlienApplication(GfxDeviceType deviceType) : m_GfxDeviceType(deviceType) {
#ifdef LIB_GLFW
        glfwInit();
#endif
    }

    ~AlienApplication() {
        destroyWindow();
#ifdef LIB_GLFW
        glfwTerminate();
#endif
    }

    bool create(unsigned int width, unsigned int height, const char *title) {
#ifdef LIB_GLFW
        destroyWindow();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);

        GLFWimage glfwImage;

        Image image("../res/logo_128x128.png", true);
        auto imageSize = image.getImageSize();

        glfwImage.pixels = image.getData().get();
        glfwImage.width = imageSize.first;
        glfwImage.height = imageSize.second;

        glfwSetWindowIcon(m_Window, 1, &glfwImage);

        WIDTH = width;
        HEIGHT = height;

        if (!m_Window) {
            return false;
        }

//            glfwSetScrollCallback(m_Window,&AlienApplication::scrollCallback);
        glfwMakeContextCurrent(m_Window);

        ALIEN_INFO("Initializing GFX...");
        if (m_GfxDeviceType == GFX_OGL) {
            GLFWImguiAdapter::ImguiImplGlfw(this->m_Window);
            GfxBase::CreateGfxDevice(this->m_GfxDeviceType);
        } else if (m_GfxDeviceType == GFX_VK) {
            // Vulkan..
        }
#endif
        return true;
    }

    void start(const std::function<void()> &initCallback, const std::function<void()> &updateCallback,
               const std::function<void()> &keyboardCallback, bool overrideKeyword = false) {
#ifdef LIB_GLFW
        Camera.setWindowSize(WIDTH, HEIGHT);
        initCallback();
        glEnable(GL_DEPTH_TEST);

        static float lastFrame = 0.0f;
        bool mouseFlag = false, keyboardFlag = false;
        while (!m_ShouldClose && !glfwWindowShouldClose(m_Window)) {
            // TODO: Make changes (TAO etc.)
            // Calculating the deltatime as simple as possible.
            float currentFrame = static_cast<float>(glfwGetTime());
            Time = currentFrame;
            DeltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // World camera input handling..
            whenMouseOverImguiWindows(keyboardFlag, mouseFlag);
            if (!mouseFlag && m_MouseMoveable) {
                mouseInputHandling();
                if (!keyboardFlag) {
                    if (overrideKeyword) {
                        keyboardCallback();
                    } else {
                        keyboardInputHandling();
                    }
                }
                Camera.cameraScrollEvent(m_OffsetX, m_OffsetY);
            } else {
                if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
                    m_MouseMoveable = true;
            }

            auto color = Camera.getCameraBackground();
            glClearColor(color.x, color.y, color.z, color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//            glClearDepth(1.0f);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_ALWAYS);
            glCheckError();

            Camera.updateCamera();

            updateCallback();

            // draw calls
            RenderQueue::getInstance().render();

            //imgui draw calls
            GLFWImguiAdapter::ImGuiUpdate([]() {
                EditorWindow::DrawEditorWindow(DeltaTime, Camera.isOrtho());
            });

            glfwGetFramebufferSize(m_Window, &WIDTH, &HEIGHT);
            glViewport(0, 0, WIDTH, HEIGHT);
            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }
#endif
    }

    WorldCamera getWorldCamera() const {
        return Camera;
    }


    void destroyWindow() {
#ifdef LIB_GLFW
        if (m_Window) {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }
#endif
    }

    void close() {
        m_ShouldClose = true;
    }

#ifdef LIB_GLFW

    GLFWwindow *getWindow() const {
        return m_Window;
    }

#endif

    static inline i32 WIDTH{}, HEIGHT{};
    static inline float DeltaTime{}, Time{};
    static inline WorldCamera Camera{true};
private:
    void keyboardInputHandling() {
        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_Window, true);
        }

        static double shiftImpact = 5.0f;
        if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            if (shiftImpact <= 20.0f) shiftImpact += 0.5f;

        if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            shiftImpact = 5.0f;

        if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            m_MouseMoveable = false;

        if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
            m_MouseMoveable = true;


        if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
            Camera.cameraMoveForwardEvent(AlienApplication::DeltaTime * shiftImpact);
        if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
            Camera.cameraMoveBackwardEvent(AlienApplication::DeltaTime * shiftImpact);
        if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
            Camera.cameraMoveLeftEvent(AlienApplication::DeltaTime * shiftImpact);
        if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
            Camera.cameraMoveRightEvent(AlienApplication::DeltaTime * shiftImpact);
    }

    void mouseInputHandling() {
        double xPos, yPos;
        glfwGetCursorPos(m_Window, &xPos, &yPos);

        Camera.cameraFreeLookEvent(xPos, yPos);
    }

    void scrollCallback(GLFWwindow *window, double x, double y) {
        m_OffsetX = x;
        m_OffsetY = y;
    }

    void whenMouseOverImguiWindows(bool &keyboardBusy, bool &mouseBusy) {
        ImGuiIO &io = ImGui::GetIO();

        keyboardBusy = io.WantCaptureKeyboard;
        mouseBusy = io.WantCaptureMouse;
    }

#ifdef LIB_GLFW
    GLFWwindow *m_Window = nullptr;
#endif
    GfxDeviceType m_GfxDeviceType;
    bool m_ShouldClose = false;
    bool m_MouseMoveable = true;
    double m_OffsetX, m_OffsetY;
    //float m_DeltaTime {0.0f};
};

#endif
