#include "Application.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace xauusd::ui {

Application::Application() = default;

Application::~Application() {
    shutdown();
}

bool Application::initialize(int width, int height, const char* title) {
    if (running_) {
        return true;
    }

    if (!window_.create(width, height, title)) {
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    imgui_initialized_ = true;

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    apply_default_theme();

    if (!ImGui_ImplGlfw_InitForOpenGL(window_.handle(), true)) {
        ImGui::DestroyContext();
        imgui_initialized_ = false;
        window_.destroy();
        return false;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        imgui_initialized_ = false;
        window_.destroy();
        return false;
    }

    running_ = true;
    set_notification(state_, "Control Center initialized");
    return true;
}

void Application::run() {
    if (!running_) {
        return;
    }

    while (!window_.should_close() && !state_.exit_requested) {
        window_.poll_events();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        layout_.render(state_);

        ImGui::Render();
        int display_w = 0;
        int display_h = 0;
        glfwGetFramebufferSize(window_.handle(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.045f, 0.050f, 0.060f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window_.swap_buffers();
    }

    running_ = false;
}

void Application::shutdown() {
    if (imgui_initialized_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        imgui_initialized_ = false;
    }

    window_.destroy();
    running_ = false;
}

UiState& Application::state() noexcept {
    return state_;
}

MainLayout& Application::layout() noexcept {
    return layout_;
}

} // namespace xauusd::ui
