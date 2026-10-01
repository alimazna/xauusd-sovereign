#include "BarFinalizer.h"
#include "ValidationOutcome.h"
#include "DataQualityState.h"
namespace xauusd::sovereign {
BarFinalizer::BarFinalizer(const IDataValidator* validator) noexcept:validator_(validator){}
BarFinalizationState BarFinalizer::process_bar(Timeframe timeframe,const Bar&bar,DataValidationResult& out_validation){
 if(validator_==nullptr){out_validation=DataValidationResult{};return BarFinalizationState::UNKNOWN;}
 if(bar.timeframe!=timeframe){out_validation=DataValidationResult{ValidationOutcome::REJECTED,DataQualityState::INVALID,"bar timeframe does not match caller timeframe",bar.close_time};return BarFinalizationState::REJECTED;}
 out_validation=validator_->validate_bar(bar);
 switch(out_validation.outcome){case ValidationOutcome::ACCEPTED:case ValidationOutcome::PARTIAL:return BarFinalizationState::FINALIZED;case ValidationOutcome::REJECTED:case ValidationOutcome::QUARANTINED:return BarFinalizationState::REJECTED;case ValidationOutcome::UNKNOWN_VALUE:default:return BarFinalizationState::UNKNOWN;}
}
}
