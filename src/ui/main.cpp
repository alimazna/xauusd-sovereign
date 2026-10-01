#include "core/Application.h"

int main() {
    xauusd::ui::Application app;
    if (!app.initialize(1600, 900, "XAUUSD Sovereign — Control Center")) {
        return 1;
    }
    app.run();
    app.shutdown();
    return 0;
}
