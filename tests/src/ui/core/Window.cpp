#include "Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace xauusd::ui {

Window::~Window() {
    destroy();
}

bool Window::create(int width, int height, const char* title) {
    if (handle_ != nullptr || !glfwInit()) {
        return handle_ != nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    handle_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (handle_ == nullptr) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(handle_);
    glfwSwapInterval(1);

    const auto loaded = gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    if (loaded == 0) {
        glfwDestroyWindow(handle_);
        handle_ = nullptr;
        glfwTerminate();
        return false;
    }

    return true;
}

bool Window::should_close() const {
    return handle_ == nullptr || glfwWindowShouldClose(handle_) != 0;
}

void Window::poll_events() {
    glfwPollEvents();
}

void Window::swap_buffers() {
    if (handle_ != nullptr) {
        glfwSwapBuffers(handle_);
    }
}

void Window::destroy() {
    if (handle_ != nullptr) {
        glfwDestroyWindow(handle_);
        handle_ = nullptr;
        glfwTerminate();
    }
}

GLFWwindow* Window::handle() const noexcept {
    return handle_;
}

} // namespace xauusd::ui
