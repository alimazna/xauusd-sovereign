#include "ShadowExecutionEngine.h"
#include "EngineIdentity.h"
#include <cmath>
#include <algorithm>
namespace xauusd::sovereign {
SimulatedFill ShadowExecutionEngine::execute(const RiskProposal&p,double bid,double ask,MarketQuality q){
 SimulatedFill f;
 f.proposal_id=p.proposal_id; f.signal_id=p.signal_id; f.fill_time=p.proposed_at; f.direction=p.direction;
 f.fill_id=detail::derive_id("fill",p.proposal_id,static_cast<std::uint64_t>(p.proposed_at.value()),static_cast<std::uint64_t>(p.volume*1000.0));
 f.stop_loss=p.stop_loss; f.take_profit=p.take_profit; f.tick_size=p.tick_size; f.tick_value=p.tick_value;
 if(!p.approved||p.volume<=0||q==MarketQuality::POOR||q==MarketQuality::UNKNOWN){f.state=FillState::REJECTED;return f;}
 if(!std::isfinite(bid)||!std::isfinite(ask)||ask<bid||p.tick_size<=0||p.tick_value<=0){f.state=FillState::REJECTED;return f;}
 const double spread=std::max(0.0,ask-bid);
 f.requested_price=p.entry_price;
 const double execution_buffer=spread*0.5+fixed_buffer_;
 f.fill_price=(p.direction==SignalDirection::LONG)?ask+execution_buffer:bid-execution_buffer;
 f.slippage=std::abs(f.fill_price-f.requested_price);
 f.volume=p.volume; f.commission=p.volume*commission_per_lot_; f.state=FillState::FILLED; return f;
}
}
