#pragma once
#include "ui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <memory>

class App {
public:
    App();
    ~App();
    void Run();

private:
    bool Init();
    void Shutdown();

    GLFWwindow* window;
    ImGuiIO* io;
    std::unique_ptr<UI> ui;
};
