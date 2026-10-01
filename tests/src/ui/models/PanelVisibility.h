#pragma once

namespace xauusd::ui {

struct PanelVisibility {
    bool dashboard{true};
    bool market{true};
    bool predictions{true};
    bool approval{true};
    bool incidents{true};
    bool research{false};
    bool knowledge{false};
    bool candidates{false};
    bool validation{false};
    bool schedule{false};
    bool about{false};
};

} // namespace xauusd::ui
