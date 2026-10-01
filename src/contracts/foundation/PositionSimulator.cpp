#include "PositionSimulator.h"
#include "EngineIdentity.h"
#include "Clock.h"
#include <cmath>
namespace xauusd::sovereign {
Position PositionSimulator::open(const SimulatedFill&f){
 Position p;
 p.position_id=detail::derive_id("position",f.fill_id,static_cast<std::uint64_t>(f.fill_time.value()),static_cast<std::uint64_t>(f.direction));
 p.signal_id=f.signal_id; p.proposal_id=f.proposal_id; p.fill_id=f.fill_id; p.direction=f.direction;
 p.state=f.state==FillState::FILLED?PositionState::OPEN:PositionState::REJECTED; p.opened_at=f.fill_time; p.requested_price=f.requested_price; p.entry_price=f.fill_price;
 p.volume=f.volume; p.stop_loss=f.stop_loss; p.slippage=f.slippage; p.take_profit=f.take_profit; p.commission=f.commission; p.swap=f.swap;
 p.tick_size=f.tick_size; p.tick_value=f.tick_value; p.is_open=(p.state==PositionState::OPEN); return p;
}
bool PositionSimulator::update(Position&p,const Tick&t){
 if (!p.is_open) return false;
 if(!std::isfinite(t.bid)||!std::isfinite(t.ask)) return false;
 const double px=p.direction==SignalDirection::LONG?t.bid:t.ask;
 if(!std::isfinite(px)||p.volume<=0.0||p.tick_size<=0.0||p.tick_value<=0.0)return false;
 if((p.direction==SignalDirection::LONG&&p.stop_loss>0.0&&px<=p.stop_loss)||(p.direction==SignalDirection::SHORT&&p.stop_loss>0.0&&px>=p.stop_loss)){close(p,p.stop_loss,"SL",t.event_time);return true;}
 if((p.direction==SignalDirection::LONG&&p.take_profit>0.0&&px>=p.take_profit)||(p.direction==SignalDirection::SHORT&&p.take_profit>0.0&&px<=p.take_profit)){close(p,p.take_profit,"TP",t.event_time);return true;}
 const double move=std::abs(px-p.entry_price); const double risk=std::abs(p.entry_price-p.stop_loss);
 if(risk>0.0&&move>=risk&&p.stop_loss!=p.entry_price){p.stop_loss=p.entry_price;p.state=PositionState::BREAKEVEN;}
 return false;
}
Position PositionSimulator::close(Position&p,double exit,const std::string&reason){ return close(p,exit,reason,detail::now_timestamp()); }
Position PositionSimulator::close(Position&p,double exit,const std::string&reason,Timestamp closed_at){
 if(!p.is_open)return p;
 if(p.tick_size<=0.0||p.tick_value<=0.0||!std::isfinite(exit))return p;
 const double mult=p.direction==SignalDirection::LONG?1.0:-1.0;
 p.exit_price=exit; p.gross_pnl=((exit-p.entry_price)/p.tick_size)*p.tick_value*p.volume*mult;
 p.net_pnl=p.gross_pnl-p.commission-p.swap; p.closed_at=closed_at; p.close_reason=reason; p.is_open=false; p.state=PositionState::CLOSED; return p;
}
}
