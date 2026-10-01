#include "RuntimeAdapter.h"

namespace xauusd::ui {

RuntimeAdapter::RuntimeAdapter(const xauusd::sovereign::RuntimeEngine* engine) noexcept
    : engine_(engine) {}

bool RuntimeAdapter::has_engine() const noexcept {
    return engine_ != nullptr;
}

xauusd::sovereign::RuntimeState RuntimeAdapter::snapshot() const {
    return engine_ == nullptr ? xauusd::sovereign::RuntimeState{} : engine_->snapshot();
}

void RuntimeAdapter::bind(const xauusd::sovereign::RuntimeEngine* engine) noexcept {
    engine_ = engine;
}

} // namespace xauusd::ui
