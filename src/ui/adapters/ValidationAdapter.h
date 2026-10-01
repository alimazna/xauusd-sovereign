#pragma once

#include "ValidationFirewall.h"

#include <cstddef>
#include <vector>

namespace xauusd::ui {

struct ValidationSnapshot {
    std::size_t protocol_count{0};
    std::size_t run_count{0};
    std::vector<xauusd::sovereign::ValidationResult> results;
    bool selected_run_passed{false};
    bool has_selected_run{false};
};

class ValidationAdapter {
public:
    explicit ValidationAdapter(const xauusd::sovereign::ValidationFirewall* firewall) noexcept;

    bool has_firewall() const noexcept;
    ValidationSnapshot snapshot(const xauusd::sovereign::EntityId* selected_run) const;

    void bind(const xauusd::sovereign::ValidationFirewall* firewall) noexcept;

private:
    const xauusd::sovereign::ValidationFirewall* firewall_{nullptr};
};

} // namespace xauusd::ui
