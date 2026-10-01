#include "../src/contracts/foundation/EntityId.h"
#include "../src/contracts/foundation/Timestamp.h"
#include "../src/contracts/foundation/Version.h"

#include <array>
#include <cstdint>

using namespace xauusd::sovereign;

int main() {
    std::array<std::uint8_t, 16> bytes{};
    EntityId id1{bytes};
    EntityId id2{bytes};

    Timestamp t1{1000000};
    Timestamp t2{1000000};

    Version v1{42};
    Version v2{42};

    if (!(id1 == id2)) return 1;
    if (!(t1 == t2))  return 2;
    if (!(v1 == v2))  return 3;

    return 0;
}
