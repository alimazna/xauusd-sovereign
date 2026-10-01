#pragma once

#include "RuntimeEngine.h"
#include "RuntimeState.h"

namespace xauusd::ui {

class RuntimeAdapter {
public:
    explicit RuntimeAdapter(const xauusd::sovereign::RuntimeEngine* engine) noexcept;
    bool has_engine() const noexcept;
    xauusd::sovereign::RuntimeState snapshot() const;

    void bind(const xauusd::sovereign::RuntimeEngine* engine) noexcept;

private:
    const xauusd::sovereign::RuntimeEngine* engine_{nullptr};
};

} // namespace xauusd::ui
