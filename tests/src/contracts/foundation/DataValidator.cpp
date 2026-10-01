#include "DataValidator.h"
#include "ValidationOutcome.h"
#include "DataQualityState.h"
#include <cmath>
namespace xauusd::sovereign {
DataValidationResult DataValidator::validate_bar(const Bar& bar) const {
 DataValidationResult result; result.observed_at=bar.close_time; result.reason=""; result.quality=DataQualityState::INVALID_QUALITY; result.outcome=ValidationOutcome::REJECTED_VALIDATION;
 if(bar.close_time.value()<=bar.open_time.value()){result.reason="bar time range is not strictly positive";return result;}
 if(!std::isfinite(bar.open)||!std::isfinite(bar.high)||!std::isfinite(bar.low)||!std::isfinite(bar.close)){result.reason="non-finite price value";return result;}
 if(bar.high<bar.low||bar.high<bar.open||bar.high<bar.close||bar.low>bar.open||bar.low>bar.close){result.reason="invalid OHLC geometry";return result;}
 if(bar.open<=0.0||bar.high<=0.0||bar.low<=0.0||bar.close<=0.0){result.reason="non-positive price";return result;}
 const auto tf=static_cast<unsigned>(bar.timeframe); if(tf>static_cast<unsigned>(Timeframe::MN1)){result.reason="invalid timeframe";return result;}
 result.outcome=ValidationOutcome::ACCEPTED_VALIDATION;result.quality=DataQualityState::VALID_QUALITY;return result;
}
DataValidationResult DataValidator::validate_tick(const Tick& tick) const {
 DataValidationResult result; result.observed_at=tick.receive_time; result.quality=DataQualityState::INVALID_QUALITY; result.outcome=ValidationOutcome::REJECTED_VALIDATION;
 if(tick.receive_time.value()<tick.event_time.value()){result.reason="receive_time precedes event_time";return result;}
 if(!std::isfinite(tick.bid)||!std::isfinite(tick.ask)||!std::isfinite(tick.last)||!std::isfinite(tick.volume)){result.reason="non-finite tick value";return result;}
 if(tick.bid<=0.0||tick.ask<=0.0){result.reason="non-positive bid or ask";return result;}
 if(tick.ask<tick.bid){result.reason="ask below bid";return result;}
 if(tick.volume<0.0){result.reason="negative volume";return result;}
 result.outcome=ValidationOutcome::ACCEPTED_VALIDATION;result.quality=DataQualityState::VALID_QUALITY;return result;
}
DataValidationResult DataValidator::validate_symbol_spec(const SymbolSpec& spec) const {
 DataValidationResult result; result.observed_at=spec.observed_at; result.quality=DataQualityState::INVALID_QUALITY; result.outcome=ValidationOutcome::REJECTED_VALIDATION;
 if(spec.symbol.empty()){result.reason="empty symbol";return result;}
 if(spec.digits==0){result.reason="zero digits";return result;}
 const double vals[]={spec.point,spec.tick_size,spec.tick_value,spec.contract_size,spec.volume_min,spec.volume_max,spec.volume_step,spec.stops_level,spec.freeze_level};
 for(const double v:vals)if(!std::isfinite(v)){result.reason="non-finite symbol specification";return result;}
 if(spec.point<=0.0||spec.tick_size<=0.0||spec.tick_value<=0.0){result.reason="non-positive point/tick_size/tick_value";return result;}
 if(spec.contract_size<=0.0){result.reason="non-positive contract_size";return result;}
 if(spec.volume_min<=0.0||spec.volume_max<spec.volume_min||spec.volume_step<=0.0){result.reason="invalid volume limits";return result;}
 if(spec.stops_level<0.0||spec.freeze_level<0.0){result.reason="negative stop/freeze level";return result;}
 result.outcome=ValidationOutcome::ACCEPTED_VALIDATION;result.quality=DataQualityState::VALID_QUALITY;return result;
}
}
