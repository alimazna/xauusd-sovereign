#include "MT5MessageParser.h"

#include <chrono>
#include <cmath>
#include <cctype>
#include <charconv>
#include <cstdlib>
#include <limits>
#include <string_view>

namespace xauusd::mt5 {
namespace {

constexpr std::int64_t kMissingInt64 = std::numeric_limits<std::int64_t>::min();

void skip_ws(std::string_view text, std::size_t& pos) {
    while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos]))) {
        ++pos;
    }
}

bool parse_quoted(std::string_view text, std::size_t& pos, std::string& out) {
    skip_ws(text, pos);
    if (pos >= text.size() || text[pos] != '"') {
        return false;
    }
    ++pos;
    out.clear();
    while (pos < text.size()) {
        const char ch = text[pos++];
        if (ch == '"') {
            return true;
        }
        if (ch != '\\') {
            out.push_back(ch);
            continue;
        }
        if (pos >= text.size()) {
            return false;
        }
        const char esc = text[pos++];
        switch (esc) {
            case '"': out.push_back('"'); break;
            case '\\': out.push_back('\\'); break;
            case '/': out.push_back('/'); break;
            case 'b': out.push_back('\b'); break;
            case 'f': out.push_back('\f'); break;
            case 'n': out.push_back('\n'); break;
            case 'r': out.push_back('\r'); break;
            case 't': out.push_back('\t'); break;
            case 'u': {
                // Decode BMP \\uXXXX escapes sufficiently for flat protocol strings.
                if (pos + 4 > text.size()) return false;
                unsigned value = 0;
                for (int i = 0; i < 4; ++i) {
                    const char h = text[pos++];
                    value <<= 4;
                    if (h >= '0' && h <= '9') value += static_cast<unsigned>(h - '0');
                    else if (h >= 'a' && h <= 'f') value += static_cast<unsigned>(10 + h - 'a');
                    else if (h >= 'A' && h <= 'F') value += static_cast<unsigned>(10 + h - 'A');
                    else return false;
                }
                // UTF-8 encode BMP code point.
                if (value < 0x80) {
                    out.push_back(static_cast<char>(value));
                } else if (value < 0x800) {
                    out.push_back(static_cast<char>(0xC0 | (value >> 6)));
                    out.push_back(static_cast<char>(0x80 | (value & 0x3F)));
                } else {
                    out.push_back(static_cast<char>(0xE0 | (value >> 12)));
                    out.push_back(static_cast<char>(0x80 | ((value >> 6) & 0x3F)));
                    out.push_back(static_cast<char>(0x80 | (value & 0x3F)));
                }
                break;
            }
            default:
                return false;
        }
    }
    return false;
}

bool parse_json_string_value(const std::string& json, const std::string& key, std::string& out) {
    std::string_view text(json);
    std::size_t pos = 0;
    while (pos < text.size()) {
        skip_ws(text, pos);
        if (pos >= text.size()) return false;
        if (text[pos] == '{') { ++pos; continue; }
        if (text[pos] != '"') return false;
        std::string parsed_key;
        if (!parse_quoted(text, pos, parsed_key)) return false;
        skip_ws(text, pos);
        if (pos >= text.size() || text[pos] != ':') return false;
        ++pos;
        skip_ws(text, pos);
        if (parsed_key == key) {
            return parse_quoted(text, pos, out);
        }

        if (pos < text.size() && text[pos] == '"') {
            std::string ignored;
            if (!parse_quoted(text, pos, ignored)) return false;
        } else {
            const std::size_t start = pos;
            while (pos < text.size() && text[pos] != ',' && text[pos] != '}') ++pos;
            if (start == pos) return false;
        }
        skip_ws(text, pos);
        if (pos < text.size() && text[pos] == ',') {
            ++pos;
            continue;
        }
        if (pos < text.size() && text[pos] == '}') break;
    }
    return false;
}

bool parse_raw_value(const std::string& json, const std::string& key, std::string& raw) {
    std::string_view text(json);
    std::size_t pos = 0;
    while (pos < text.size()) {
        skip_ws(text, pos);
        if (pos >= text.size()) return false;
        if (text[pos] == '{') { ++pos; continue; }
        if (text[pos] != '"') return false;
        std::string parsed_key;
        if (!parse_quoted(text, pos, parsed_key)) return false;
        skip_ws(text, pos);
        if (pos >= text.size() || text[pos] != ':') return false;
        ++pos;
        skip_ws(text, pos);
        const std::size_t start = pos;
        if (pos < text.size() && text[pos] == '"') {
            std::string ignored;
            if (!parse_quoted(text, pos, ignored)) return false;
            if (parsed_key == key) {
                // Preserve a decoded string without exposing JSON quoting.
                raw = ignored;
                return true;
            }
        } else {
            while (pos < text.size() && text[pos] != ',' && text[pos] != '}') ++pos;
            std::size_t end = pos;
            while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1]))) --end;
            if (parsed_key == key) {
                raw.assign(text.substr(start, end - start));
                return true;
            }
        }
        skip_ws(text, pos);
        if (pos < text.size() && text[pos] == ',') {
            ++pos;
            continue;
        }
        if (pos < text.size() && text[pos] == '}') break;
    }
    return false;
}

std::optional<double> optional_number(const std::string& json, const std::string& key) {
    std::string raw;
    if (!parse_raw_value(json, key, raw) || raw.empty()) return std::nullopt;
    double value = 0.0;
    const char* begin = raw.data();
    const char* end = raw.data() + raw.size();
    // strtod accepts scientific notation and is adequate for a small flat parser.
    char* parse_end = nullptr;
    value = std::strtod(begin, &parse_end);
    if (parse_end != end || !std::isfinite(value)) return std::nullopt;
    return value;
}

std::optional<std::int64_t> optional_int64(const std::string& json, const std::string& key) {
    std::string raw;
    if (!parse_raw_value(json, key, raw) || raw.empty()) return std::nullopt;
    std::int64_t value = 0;
    const auto [ptr, ec] = std::from_chars(raw.data(), raw.data() + raw.size(), value);
    if (ec != std::errc{} || ptr != raw.data() + raw.size()) return std::nullopt;
    return value;
}

std::int64_t now_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

bool required_string(const std::string& json, const char* key, std::string& out) {
    return parse_json_string_value(json, key, out);
}

template <typename T>
bool required_number(const std::string& json, const char* key, T& out);

template <>
bool required_number<double>(const std::string& json, const char* key, double& out) {
    const auto value = optional_number(json, key);
    if (!value) return false;
    out = *value;
    return true;
}

template <>
bool required_number<std::int64_t>(const std::string& json, const char* key, std::int64_t& out) {
    const auto value = optional_int64(json, key);
    if (!value) return false;
    out = *value;
    return true;
}

} // namespace

std::string MT5MessageParser::extract_string(const std::string& json, const std::string& key) {
    std::string out;
    return parse_json_string_value(json, key, out) ? out : std::string{};
}

double MT5MessageParser::extract_number(const std::string& json, const std::string& key) {
    const auto value = optional_number(json, key);
    return value ? *value : std::numeric_limits<double>::quiet_NaN();
}

std::int64_t MT5MessageParser::extract_int64(const std::string& json, const std::string& key) {
    const auto value = optional_int64(json, key);
    return value ? *value : kMissingInt64;
}

bool MT5MessageParser::extract_bool(const std::string& json, const std::string& key, bool& value) {
    std::string raw;
    if (!parse_raw_value(json, key, raw)) return false;
    if (raw == "true") { value = true; return true; }
    if (raw == "false") { value = false; return true; }
    return false;
}

std::optional<sovereign::Timeframe> MT5MessageParser::parse_timeframe(const std::string& value) {
    if (value == "M1") return sovereign::Timeframe::M1;
    if (value == "M5") return sovereign::Timeframe::M5;
    if (value == "M15") return sovereign::Timeframe::M15;
    if (value == "M30") return sovereign::Timeframe::M30;
    if (value == "H1") return sovereign::Timeframe::H1;
    if (value == "H4") return sovereign::Timeframe::H4;
    if (value == "D1") return sovereign::Timeframe::D1;
    if (value == "W1") return sovereign::Timeframe::W1;
    if (value == "MN1") return sovereign::Timeframe::MN1;
    return std::nullopt;
}

std::optional<sovereign::Tick> MT5MessageParser::parse_tick(const std::string& json) {
    std::string tf, symbol;
    std::int64_t event_time = 0;
    double bid = 0.0, ask = 0.0, last = 0.0, volume = 0.0;
    std::int64_t flags = 0;
    if (!required_string(json, "tf", tf) || !required_string(json, "sym", symbol) ||
        !required_number(json, "et", event_time) || !required_number(json, "bid", bid) ||
        !required_number(json, "ask", ask) || !required_number(json, "last", last) ||
        !required_number(json, "vol", volume) || !required_number(json, "flg", flags)) {
        return std::nullopt;
    }
    if (!parse_timeframe(tf)) return std::nullopt;
    (void)symbol;
    return sovereign::Tick{sovereign::Timestamp{event_time}, sovereign::Timestamp{now_ms()},
                           bid, ask, last, volume, static_cast<std::uint32_t>(flags)};
}

std::optional<sovereign::Bar> MT5MessageParser::parse_bar(const std::string& json) {
    std::string tf, symbol;
    std::int64_t open_time = 0, close_time = 0, tick_volume = 0, real_volume = 0;
    double open = 0.0, high = 0.0, low = 0.0, close = 0.0;
    if (!required_string(json, "tf", tf) || !required_string(json, "sym", symbol) ||
        !required_number(json, "ot", open_time) || !required_number(json, "ct", close_time) ||
        !required_number(json, "o", open) || !required_number(json, "h", high) ||
        !required_number(json, "l", low) || !required_number(json, "c", close) ||
        !required_number(json, "tv", tick_volume) || !required_number(json, "rv", real_volume)) {
        return std::nullopt;
    }
    const auto timeframe = parse_timeframe(tf);
    if (!timeframe) return std::nullopt;
    (void)symbol;
    return sovereign::Bar{sovereign::Timestamp{open_time}, sovereign::Timestamp{close_time},
                          open, high, low, close,
                          static_cast<std::uint64_t>(tick_volume),
                          static_cast<std::uint64_t>(real_volume), *timeframe};
}

std::optional<sovereign::SymbolSpec> MT5MessageParser::parse_symbol_spec(const std::string& json) {
    std::string broker, server, symbol;
    std::int64_t digits = 0;
    double point = 0.0, tick_size = 0.0, tick_value = 0.0, contract_size = 0.0;
    double volume_min = 0.0, volume_max = 0.0, volume_step = 0.0;
    double stops_level = 0.0, freeze_level = 0.0;
    if (!required_string(json, "broker", broker) || !required_string(json, "server", server) ||
        !required_string(json, "sym", symbol) || !required_number(json, "digits", digits) ||
        !required_number(json, "point", point) || !required_number(json, "tick_size", tick_size) ||
        !required_number(json, "tick_value", tick_value) || !required_number(json, "contract_size", contract_size) ||
        !required_number(json, "volume_min", volume_min) || !required_number(json, "volume_max", volume_max) ||
        !required_number(json, "volume_step", volume_step) || !required_number(json, "stops_level", stops_level) ||
        !required_number(json, "freeze_level", freeze_level)) {
        return std::nullopt;
    }
    return sovereign::SymbolSpec{std::move(broker), std::move(server), std::move(symbol),
                                 static_cast<std::uint32_t>(digits), point, tick_size, tick_value,
                                 contract_size, volume_min, volume_max, volume_step,
                                 stops_level, freeze_level, sovereign::Timestamp{now_ms()}};
}

} // namespace xauusd::mt5
