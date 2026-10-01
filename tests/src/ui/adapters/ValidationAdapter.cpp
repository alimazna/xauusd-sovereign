#include "ValidationAdapter.h"

namespace xauusd::ui {

ValidationAdapter::ValidationAdapter(const xauusd::sovereign::ValidationFirewall* firewall) noexcept
    : firewall_(firewall) {}

bool ValidationAdapter::has_firewall() const noexcept {
    return firewall_ != nullptr;
}

ValidationSnapshot ValidationAdapter::snapshot(
    const xauusd::sovereign::EntityId* selected_run) const {
    ValidationSnapshot result;
    if (firewall_ == nullptr) {
        return result;
    }

    result.protocol_count = firewall_->protocol_count();
    result.run_count = firewall_->run_count();
    if (selected_run != nullptr) {
        result.results = firewall_->results_for(*selected_run);
        result.selected_run_passed = firewall_->all_passed(*selected_run);
        result.has_selected_run = true;
    }
    return result;
}

void ValidationAdapter::bind(const xauusd::sovereign::ValidationFirewall* firewall) noexcept {
    firewall_ = firewall;
}

} // namespace xauusd::ui
