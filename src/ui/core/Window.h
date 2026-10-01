#pragma once

struct GLFWwindow;

namespace xauusd::ui {

class Window {
public:
    Window() = default;
    ~Window();

    bool create(int width, int height, const char* title);
    bool should_close() const;
    void poll_events();
    void swap_buffers();
    void destroy();

    GLFWwindow* handle() const noexcept;

private:
    GLFWwindow* handle_{nullptr};
};

} // namespace xauusd::ui
