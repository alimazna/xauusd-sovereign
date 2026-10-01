#pragma once
#include <cstdint>
namespace xauusd::sovereign {
enum class StructureType:std::uint8_t{NONE,HIGHER_HIGH,HIGHER_LOW,LOWER_HIGH,LOWER_LOW,BREAK_OF_STRUCTURE_UP,BREAK_OF_STRUCTURE_DOWN,CHANNEL_UP,CHANNEL_DOWN,RANGE};
}
