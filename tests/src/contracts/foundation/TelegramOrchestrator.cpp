#include "TelegramOrchestrator.h"
#include "Clock.h"
#include "EngineIdentity.h"
namespace xauusd::sovereign {
bool TelegramOrchestrator::send_status(const std::string&chat_id,const std::string&text){if(chat_id.empty()||text.empty())return false;const auto now=detail::now_timestamp();const TelegramMessage message(detail::make_id("telegram_status",static_cast<std::uint64_t>(now.value()),static_cast<std::uint64_t>(text.size()),static_cast<std::uint64_t>(chat_id.size())),TelegramMessageType::STATUS_UPDATE,chat_id,text,now,false,std::string{});return gateway_.send(message);}
bool TelegramOrchestrator::send_alert(NotificationPriority priority,const std::string&title,const std::string&body){if(title.empty()&&body.empty())return false;const auto now=detail::now_timestamp();const TelegramAlert alert(detail::make_id("telegram_alert",static_cast<std::uint64_t>(now.value()),static_cast<std::uint64_t>(title.size()),static_cast<std::uint64_t>(body.size())),priority,title,body,now,false);return gateway_.send_alert(alert);}
bool TelegramOrchestrator::enqueue_notification(const NotificationRecord&record){return notifications_.enqueue(record);}
bool TelegramOrchestrator::mark_notification_delivered(const EntityId&notification_id){return notifications_.mark_delivered(notification_id);}
std::size_t TelegramOrchestrator::undelivered_count()const{return notifications_.undelivered().size();}
bool TelegramOrchestrator::gateway_available()const{return gateway_.is_available();}
void TelegramOrchestrator::set_live_delivery(bool enabled){gateway_.set_live_delivery(enabled);}
void TelegramOrchestrator::clear(){gateway_.clear();notifications_.clear();}
}
