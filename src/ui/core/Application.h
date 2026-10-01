#pragma once

#include "Window.h"
#include "Theme.h"
#include "../layout/MainLayout.h"
#include "../models/UiState.h"

namespace xauusd::ui {

class Application {
public:
    Application();
    ~Application();

    bool initialize(int width, int height, const char* title);
    void run();
    void shutdown();

    UiState& state() noexcept;
    MainLayout& layout() noexcept;

private:
    Window window_;
    UiState state_;
    MainLayout layout_;
    bool running_{false};
    bool imgui_initialized_{false};
};

} // namespace xauusd::ui
