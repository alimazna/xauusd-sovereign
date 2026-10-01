#include "BarFinalizer.h"
#include "ValidationOutcome.h"
#include "DataQualityState.h"
namespace xauusd::sovereign {
BarFinalizer::BarFinalizer(const IDataValidator* validator) noexcept:validator_(validator){}
BarFinalizationState BarFinalizer::process_bar(Timeframe timeframe,const Bar&bar,DataValidationResult& out_validation){
 if(validator_==nullptr){out_validation=DataValidationResult{};return BarFinalizationState::UNKNOWN;}
 if(bar.timeframe!=timeframe){out_validation=DataValidationResult{ValidationOutcome::REJECTED_VALIDATION,DataQualityState::INVALID_QUALITY,"bar timeframe does not match caller timeframe",bar.close_time};return BarFinalizationState::REJECTED;}
 out_validation=validator_->validate_bar(bar);
 switch(out_validation.outcome){case ValidationOutcome::ACCEPTED_VALIDATION:case ValidationOutcome::PARTIAL_VALIDATION:return BarFinalizationState::FINALIZED;case ValidationOutcome::REJECTED_VALIDATION:case ValidationOutcome::QUARANTINED_VALIDATION:return BarFinalizationState::REJECTED;case ValidationOutcome::UNKNOWN_VALIDATION_VALUE:default:return BarFinalizationState::UNKNOWN;}
}
}
