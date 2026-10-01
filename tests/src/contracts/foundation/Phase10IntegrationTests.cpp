#include "NotificationService.h"
#include "TelegramGateway.h"
#include "TelegramOrchestrator.h"

#include <array>
#include <cstdint>
#include <string>

namespace xauusd::sovereign::tests {
namespace {
#define CHECK(expr) do { if (!(expr)) { return false; } } while (false)

static EntityId make_id(std::uint8_t seed) {
    std::array<std::uint8_t, 16> bytes{};
    bytes[0] = seed;
    return EntityId{bytes};
}

static TelegramMessage make_message(std::uint8_t seed) {
    return TelegramMessage(
        make_id(seed), TelegramMessageType::REPORT, "chat", "text",
        Timestamp(1), false, "");
}

static TelegramAlert make_alert(std::uint8_t seed) {
    return TelegramAlert(
        make_id(seed), NotificationPriority::HIGH, "title", "body",
        Timestamp(2), false);
}

static NotificationRecord make_record(std::uint8_t seed, bool delivered = false) {
    return NotificationRecord(
        make_id(seed), NotificationChannel::TELEGRAM,
        NotificationPriority::NORMAL, "title", "body",
        Timestamp(3), Timestamp(0), delivered);
}

static bool test_1_gateway_send_message() {
    TelegramGateway gateway;
    gateway.set_live_delivery(false);
    const auto message = make_message(1);
    CHECK(gateway.send(message));
    CHECK(gateway.recent_messages().size() == 1);
    CHECK(gateway.recent_messages().front().sent);
    return true;
}

static bool test_2_gateway_send_alert() {
    TelegramGateway gateway;
    gateway.set_live_delivery(false);
    CHECK(gateway.send_alert(make_alert(2)));
    const auto messages = gateway.recent_messages();
    CHECK(messages.size() == 1);
    CHECK(messages.front().type == TelegramMessageType::ALERT);
    CHECK(messages.front().text == "title\nbody");
    return true;
}

static bool test_3_gateway_set_next_send_result() {
    TelegramGateway gateway;
    gateway.set_live_delivery(false);
    gateway.set_next_send_result(false);
    CHECK(!gateway.send(make_message(3)));
    CHECK(gateway.send(make_message(4)));
    return true;
}

static bool test_4_gateway_messages_sent() {
    TelegramGateway gateway;
    gateway.set_live_delivery(false);
    gateway.set_next_send_result(false);
    CHECK(!gateway.send(make_message(5)));
    CHECK(gateway.send(make_message(6)));
    CHECK(gateway.send(make_message(7)));
    CHECK(gateway.messages_sent() == 2);
    return true;
}

static bool test_5_gateway_is_available() {
    TelegramGateway gateway;
    gateway.set_live_delivery(false);
    CHECK(gateway.is_available());
    gateway.set_available(false);
    CHECK(!gateway.is_available());
    return true;
}

static bool test_6_gateway_recent_messages() {
    TelegramGateway gateway;
    gateway.set_live_delivery(false);
    for (std::uint8_t i = 0; i < 105; ++i) {
        CHECK(gateway.send(make_message(static_cast<std::uint8_t>(i + 1))));
    }
    const auto messages = gateway.recent_messages();
    CHECK(messages.size() == 100);
    CHECK(messages.front().message_id.bytes()[0] == 6);
    CHECK(messages.back().message_id.bytes()[0] == 105);
    return true;
}

static bool test_7_notification_enqueue() {
    NotificationService service;
    CHECK(service.enqueue(make_record(10)));
    CHECK(service.size() == 1);
    return true;
}

static bool test_8_notification_duplicate_rejection() {
    NotificationService service;
    const auto record = make_record(11);
    CHECK(service.enqueue(record));
    CHECK(!service.enqueue(record));
    CHECK(service.size() == 1);
    return true;
}

static bool test_9_notification_mark_delivered() {
    NotificationService service;
    const auto record = make_record(12);
    CHECK(service.enqueue(record));
    CHECK(service.mark_delivered(record.notification_id));
    CHECK(service.undelivered().empty());
    CHECK(service.all().front().delivered);
    return true;
}

static bool test_10_notification_undelivered() {
    NotificationService service;
    CHECK(service.enqueue(make_record(13, false)));
    CHECK(service.enqueue(make_record(14, true)));
    CHECK(service.undelivered().size() == 1);
    CHECK(service.undelivered().front().notification_id.bytes()[0] == 13);
    return true;
}

static bool test_11_notification_all() {
    NotificationService service;
    CHECK(service.enqueue(make_record(15)));
    CHECK(service.enqueue(make_record(16)));
    CHECK(service.all().size() == 2);
    return true;
}

static bool test_12_notification_size() {
    NotificationService service;
    CHECK(service.size() == 0);
    CHECK(service.enqueue(make_record(17)));
    CHECK(service.enqueue(make_record(18)));
    CHECK(service.size() == 2);
    return true;
}

static bool test_13_notification_clear() {
    NotificationService service;
    const auto record = make_record(19);
    CHECK(service.enqueue(record));
    service.clear();
    CHECK(service.size() == 0);
    CHECK(service.undelivered().empty());
    CHECK(service.enqueue(record));
    return true;
}

static bool test_14_orchestrator_send_status() {
    TelegramOrchestrator orchestrator;
    orchestrator.set_live_delivery(false);
    CHECK(orchestrator.send_status("chat", "status"));
    CHECK(orchestrator.gateway_available());
    return true;
}

static bool test_15_orchestrator_send_alert() {
    TelegramOrchestrator orchestrator;
    orchestrator.set_live_delivery(false);
    CHECK(orchestrator.send_alert(NotificationPriority::URGENT, "alert", "body"));
    return true;
}

static bool test_16_orchestrator_enqueue_notification() {
    TelegramOrchestrator orchestrator;
    const auto record = make_record(20);
    CHECK(orchestrator.enqueue_notification(record));
    CHECK(orchestrator.undelivered_count() == 1);
    return true;
}

static bool test_17_orchestrator_mark_notification_delivered() {
    TelegramOrchestrator orchestrator;
    const auto record = make_record(21);
    CHECK(orchestrator.enqueue_notification(record));
    CHECK(orchestrator.mark_notification_delivered(record.notification_id));
    CHECK(orchestrator.undelivered_count() == 0);
    return true;
}

static bool test_18_orchestrator_undelivered_count() {
    TelegramOrchestrator orchestrator;
    CHECK(orchestrator.enqueue_notification(make_record(22)));
    CHECK(orchestrator.enqueue_notification(make_record(23)));
    CHECK(orchestrator.undelivered_count() == 2);
    return true;
}

static bool test_19_orchestrator_gateway_available() {
    TelegramOrchestrator orchestrator;
    orchestrator.set_live_delivery(false);
    CHECK(orchestrator.gateway_available());
    orchestrator.clear();
    CHECK(orchestrator.gateway_available());
    return true;
}

static bool test_20_enum_sanity() {
    CHECK(static_cast<std::uint8_t>(TelegramMessageType::STATUS_UPDATE) == 0);
    CHECK(static_cast<std::uint8_t>(TelegramMessageType::REPLY) == 6);
    CHECK(static_cast<std::uint8_t>(NotificationPriority::LOW) == 0);
    CHECK(static_cast<std::uint8_t>(NotificationPriority::CRITICAL) == 4);
    CHECK(static_cast<std::uint8_t>(NotificationChannel::TELEGRAM) == 0);
    CHECK(static_cast<std::uint8_t>(NotificationChannel::NONE) == 3);
    return true;
}

} // namespace
} // namespace xauusd::sovereign::tests

int main() {
    using namespace xauusd::sovereign::tests;
    const bool results[] = {
        test_1_gateway_send_message(), test_2_gateway_send_alert(),
        test_3_gateway_set_next_send_result(), test_4_gateway_messages_sent(),
        test_5_gateway_is_available(), test_6_gateway_recent_messages(),
        test_7_notification_enqueue(), test_8_notification_duplicate_rejection(),
        test_9_notification_mark_delivered(), test_10_notification_undelivered(),
        test_11_notification_all(), test_12_notification_size(),
        test_13_notification_clear(), test_14_orchestrator_send_status(),
        test_15_orchestrator_send_alert(), test_16_orchestrator_enqueue_notification(),
        test_17_orchestrator_mark_notification_delivered(),
        test_18_orchestrator_undelivered_count(), test_19_orchestrator_gateway_available(),
        test_20_enum_sanity()
    };
    for (bool ok : results) {
        if (!ok) return 1;
    }
    return 0;
}
