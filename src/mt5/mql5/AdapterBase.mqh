//+------------------------------------------------------------------+
//|                                              AdapterBase.mqh     |
//+------------------------------------------------------------------+
#property copyright "XAUUSD Sovereign"
#include "Common.mqh"
#include "Protocol.mqh"
#include "SocketClient.mqh"
#include "OrderExecutor.mqh"

class CAdapterBase
{
protected:
   string         m_timeframe;
   ENUM_TIMEFRAMES m_period;
   CSocketClient  m_socket;
   bool           m_connected;
   bool           m_handshake_ack;
   datetime       m_last_heartbeat;
   datetime       m_last_reconnect_attempt;
   string         m_pending_command_ids[];
   ulong          m_pending_orders[];
   ulong          m_pending_deals[];
   ulong          m_pending_positions[];

public:
   CAdapterBase(const string timeframe,const ENUM_TIMEFRAMES period)
      : m_timeframe(timeframe),m_period(period),m_connected(false),m_handshake_ack(false),
        m_last_heartbeat(0),m_last_reconnect_attempt(0) {}
   virtual ~CAdapterBase() { Deinit(); }

   bool Init()
   {
      Print("[MT5] ",m_timeframe," Adapter starting for ",_Symbol);
      return ConnectAndHandshake();
   }

   void Deinit()
   {
      if(m_connected)
      {
         string disc = "{\"tf\":\"" + JsonEscape(m_timeframe) + "\"}";
         m_socket.SendMessage(MT5_MSG_DISCONNECT,disc);
      }
      m_socket.Disconnect();
      m_connected = false;
      m_handshake_ack = false;
      Print("[MT5] ",m_timeframe," Adapter stopped");
   }

   virtual void OnTickHandler()
   {
      if(!m_connected || !m_handshake_ack) return;
      if(!SymbolInfoTick(_Symbol,m_tick)) return;
      string json = CProtocol::BuildTick(m_timeframe,_Symbol,m_tick.time_msc,
                                          m_tick.bid,m_tick.ask,m_tick.last,
                                          (double)m_tick.volume,(uint)m_tick.flags);
      if(!m_socket.SendMessage(MT5_MSG_TICK,json)) HandleSendFailure();
   }

   virtual void OnBarCloseHandler()
   {
      if(!m_connected || !m_handshake_ack) return;
      double o = iOpen(_Symbol,m_period,1);
      double h = iHigh(_Symbol,m_period,1);
      double l = iLow(_Symbol,m_period,1);
      double c = iClose(_Symbol,m_period,1);
      long tv = (long)iVolume(_Symbol,m_period,1);
      long rv = (long)iRealVolume(_Symbol,m_period,1);
      datetime ot = iTime(_Symbol,m_period,1);
      datetime ct = iTime(_Symbol,m_period,0);
      if(ct<=0) ct = ot + PeriodSeconds(m_period);
      string json = CProtocol::BuildBarClosed(m_timeframe,_Symbol,ot,ct,o,h,l,c,tv,rv);
      if(!m_socket.SendMessage(MT5_MSG_BAR_CLOSED,json)) HandleSendFailure();
   }

   virtual void OnTimerHandler()
   {
      if(!m_connected)
      {
         datetime now = TimeCurrent();
         if(now-m_last_reconnect_attempt>=5)
         {
            m_last_reconnect_attempt = now;
            ConnectAndHandshake();
         }
         return;
      }

      datetime now = TimeCurrent();
      if(InpEnableHeartbeat && now-m_last_heartbeat>=InpHeartbeatSec)
      {
         if(!m_socket.SendMessage(MT5_MSG_HEARTBEAT,CProtocol::BuildHeartbeat(m_timeframe)))
         {
            HandleSendFailure();
            return;
         }
         m_last_heartbeat = now;
      }

      // SocketRead is permitted from an Expert Advisor timer context.
      string payload;
      ushort msg_type = m_socket.ReceiveMessage(payload,10);
      if(msg_type!=0) HandleServerMessage(msg_type,payload);
   }

protected:
   MqlTick m_tick;

   bool ConnectAndHandshake()
   {
      if(!m_socket.Connect(InpServerHost,InpServerPort))
      {
         m_connected = false;
         return false;
      }
      m_connected = true;
      m_handshake_ack = false;
      if(!m_socket.SendMessage(MT5_MSG_HANDSHAKE,CProtocol::BuildHandshake(m_timeframe)))
      {
         HandleSendFailure();
         return false;
      }
      SendSymbolSpec();
      Print("[MT5] ",m_timeframe," Adapter connected");
      return true;
   }

   void SendSymbolSpec()
   {
      string broker = AccountInfoString(ACCOUNT_COMPANY);
      string server = AccountInfoString(ACCOUNT_SERVER);
      string json = CProtocol::BuildSymbolSpec(
         broker,server,_Symbol,
         (int)SymbolInfoInteger(_Symbol,SYMBOL_DIGITS),
         SymbolInfoDouble(_Symbol,SYMBOL_POINT),
         SymbolInfoDouble(_Symbol,SYMBOL_TRADE_TICK_SIZE),
         SymbolInfoDouble(_Symbol,SYMBOL_TRADE_TICK_VALUE),
         SymbolInfoDouble(_Symbol,SYMBOL_TRADE_CONTRACT_SIZE),
         SymbolInfoDouble(_Symbol,SYMBOL_VOLUME_MIN),
         SymbolInfoDouble(_Symbol,SYMBOL_VOLUME_MAX),
         SymbolInfoDouble(_Symbol,SYMBOL_VOLUME_STEP),
         (double)SymbolInfoInteger(_Symbol,SYMBOL_TRADE_STOPS_LEVEL),
         (double)SymbolInfoInteger(_Symbol,SYMBOL_TRADE_FREEZE_LEVEL));
      if(!m_socket.SendMessage(MT5_MSG_SYMBOL_SPEC,json)) HandleSendFailure();
   }

   void HandleSendFailure()
   {
      Print("[MT5] ",m_timeframe," Adapter send failure; reconnect scheduled");
      m_socket.Disconnect();
      m_connected = false;
      m_handshake_ack = false;
   }

   void SendCommandResult(const string command_id,const ulong ticket,const bool ok,const string comment)
   {
      Print("[MT5][COMMAND] tf=",m_timeframe," cmd=",command_id,
            " ticket=",ticket," ok=",ok," comment=",comment);
      if(m_connected)
         m_socket.SendMessage(MT5_MSG_COMMAND_RESULT,
                              CProtocol::BuildCommandResult(m_timeframe,command_id,ticket,ok,comment));
   }

   virtual void HandleServerMessage(const ushort msg_type,const string payload)
   {
      if(msg_type==MT5_MSG_HEARTBEAT_ACK)
      {
         return;
      }
      if(msg_type==MT5_MSG_HANDSHAKE_ACK)
      {
         string ack_tf;
         if(!CProtocol::ExtractString(payload,"tf",ack_tf) || ack_tf!=m_timeframe)
         {
            Print("[MT5] ",m_timeframe," invalid handshake ACK target");
            HandleSendFailure();
            return;
         }
         m_handshake_ack = true;
         Print("[MT5] ",m_timeframe," Handshake ACK received");
         return;
      }
      if(msg_type==MT5_MSG_SHUTDOWN)
      {
         Print("[MT5] ",m_timeframe," Server requested shutdown");
         m_socket.Disconnect();
         m_connected = false;
         return;
      }
      if(msg_type==MT5_MSG_NEW_ORDER)
      {
         ExecuteNewOrder(payload);
         return;
      }
      if(msg_type==MT5_MSG_CLOSE_ORDER)
      {
         ExecuteCloseOrder(payload);
         return;
      }
      if(msg_type==MT5_MSG_MODIFY_ORDER)
      {
         ExecuteModifyOrder(payload);
         return;
      }
      if(msg_type==MT5_MSG_CANCEL_ORDER)
      {
         ExecuteCancelOrder(payload);
         return;
      }
   }

   void ExecuteNewOrder(const string payload)
   {
      string cmd,tf,symbol,side,comment;
      double volume=0,price=0,sl=0,tp=0;
      if(!CProtocol::ExtractString(payload,"cmd",cmd) ||
         !CProtocol::ExtractString(payload,"tf",tf) ||
         !CProtocol::ExtractString(payload,"sym",symbol) ||
         !CProtocol::ExtractString(payload,"side",side) ||
         !CProtocol::ExtractDouble(payload,"vol",volume) ||
         !CProtocol::ExtractDouble(payload,"price",price) ||
         !CProtocol::ExtractDouble(payload,"sl",sl) ||
         !CProtocol::ExtractDouble(payload,"tp",tp))
      {
         SendCommandResult(cmd,0,false,"Invalid NEW_ORDER payload");
         return;
      }
      CProtocol::ExtractString(payload,"cmt",comment);
      if(tf!=m_timeframe)
      {
         SendCommandResult(cmd,0,false,"Timeframe mismatch: adapter is " + m_timeframe);
         return;
      }
      if(symbol!=_Symbol)
      {
         SendCommandResult(cmd,0,false,"Symbol mismatch: adapter is attached to " + _Symbol);
         return;
      }
      if(!InpEnableLiveOrders)
      {
         Print("[MT5][SHADOW] NEW_ORDER command logged; no broker order sent cmd=",cmd);
         SendCommandResult(cmd,0,true,"SHADOW_ACCEPTED (live orders disabled)");
         return;
      }
      ulong ticket = COrderExecutor::SendMarketOrder(cmd,symbol,side,volume,sl,tp,comment);
      SendCommandResult(cmd,ticket,ticket!=0,ticket!=0 ? "REQUEST_ACCEPTED" : "Order rejected");
   }

   void ExecuteCloseOrder(const string payload)
   {
      string cmd,tf;
      ulong ticket=0;
      if(!CProtocol::ExtractString(payload,"cmd",cmd) || !CProtocol::ExtractString(payload,"tf",tf) ||
         !CProtocol::ExtractUlong(payload,"ticket",ticket))
      {
         SendCommandResult(cmd,0,false,"Invalid CLOSE_ORDER payload");
         return;
      }
      if(tf!=m_timeframe) { SendCommandResult(cmd,0,false,"Timeframe mismatch: adapter is " + m_timeframe); return; }
      bool ok = COrderExecutor::ClosePosition(cmd,ticket);
      SendCommandResult(cmd,ticket,ok,InpEnableLiveOrders ? (ok ? "REQUEST_ACCEPTED" : "Close failed") : "SHADOW_ACCEPTED (live orders disabled)");
   }

   void ExecuteModifyOrder(const string payload)
   {
      string cmd,tf;
      ulong ticket=0;
      double sl=0,tp=0;
      if(!CProtocol::ExtractString(payload,"cmd",cmd) || !CProtocol::ExtractString(payload,"tf",tf) ||
         !CProtocol::ExtractUlong(payload,"ticket",ticket) ||
         !CProtocol::ExtractDouble(payload,"sl",sl) || !CProtocol::ExtractDouble(payload,"tp",tp))
      {
         SendCommandResult(cmd,0,false,"Invalid MODIFY_ORDER payload");
         return;
      }
      if(tf!=m_timeframe) { SendCommandResult(cmd,0,false,"Timeframe mismatch: adapter is " + m_timeframe); return; }
      bool ok = COrderExecutor::ModifyPosition(cmd,ticket,sl,tp);
      SendCommandResult(cmd,ticket,ok,InpEnableLiveOrders ? (ok ? "REQUEST_ACCEPTED" : "Modify failed") : "SHADOW_ACCEPTED (live orders disabled)");
   }

   void ExecuteCancelOrder(const string payload)
   {
      string cmd,tf;
      ulong ticket=0;
      if(!CProtocol::ExtractString(payload,"cmd",cmd) || !CProtocol::ExtractString(payload,"tf",tf) ||
         !CProtocol::ExtractUlong(payload,"ticket",ticket))
      {
         SendCommandResult(cmd,0,false,"Invalid CANCEL_ORDER payload");
         return;
      }
      if(tf!=m_timeframe) { SendCommandResult(cmd,0,false,"Timeframe mismatch: adapter is " + m_timeframe); return; }
      bool ok = COrderExecutor::CancelOrder(cmd,ticket);
      SendCommandResult(cmd,ticket,ok,InpEnableLiveOrders ? (ok ? "REQUEST_ACCEPTED" : "Cancel failed") : "SHADOW_ACCEPTED (live orders disabled)");
   }
   void OnTradeTransactionHandler(const MqlTradeTransaction &trans,
                                  const MqlTradeRequest &request,
                                  const MqlTradeResult &result)
   {
      string command_id = "";
      if(trans.type==TRADE_TRANSACTION_REQUEST)
      {
         if(StringFind(request.comment,"[XAUS_CMD:",0)==0)
         {
            int start=StringLen("[XAUS_CMD:");
            int end=StringFind(request.comment,"]",start);
            if(end>start) command_id=StringSubstr(request.comment,start,end-start);
         }
         if(command_id!="")
         {
            TrackCommand(command_id,trans.order!=0 ? trans.order : result.order,
                         trans.deal!=0 ? trans.deal : result.deal,
                         trans.position);
         }
      }
      else
      {
         command_id=FindCommand(trans.order,trans.deal,trans.position);
      }
      if(command_id=="") return;

      bool ok = (result.retcode==0 || result.retcode==TRADE_RETCODE_DONE ||
                 result.retcode==TRADE_RETCODE_PLACED || result.retcode==TRADE_RETCODE_DONE_PARTIAL);
      if(m_connected)
         m_socket.SendMessage(MT5_MSG_COMMAND_RESULT,
            CProtocol::BuildTradeTransaction(m_timeframe,command_id,ok,result.comment,
               (int)trans.type,trans.order,trans.deal,trans.position,result.retcode));
   }

   void TrackCommand(const string command_id,const ulong order_ticket,const ulong deal_ticket,const ulong position_ticket)
   {
      int index=FindCommandIndex(order_ticket,deal_ticket,position_ticket);
      if(index<0)
      {
         int size=ArraySize(m_pending_command_ids);
         ArrayResize(m_pending_command_ids,size+1);
         ArrayResize(m_pending_orders,size+1);
         ArrayResize(m_pending_deals,size+1);
         ArrayResize(m_pending_positions,size+1);
         index=size;
      }
      m_pending_command_ids[index]=command_id;
      if(order_ticket!=0) m_pending_orders[index]=order_ticket;
      if(deal_ticket!=0) m_pending_deals[index]=deal_ticket;
      if(position_ticket!=0) m_pending_positions[index]=position_ticket;
   }

   int FindCommandIndex(const ulong order_ticket,const ulong deal_ticket,const ulong position_ticket)
   {
      int size=ArraySize(m_pending_command_ids);
      for(int i=0;i<size;++i)
      {
         if((order_ticket!=0 && m_pending_orders[i]==order_ticket) ||
            (deal_ticket!=0 && m_pending_deals[i]==deal_ticket) ||
            (position_ticket!=0 && m_pending_positions[i]==position_ticket))
            return i;
      }
      return -1;
   }

   string FindCommand(const ulong order_ticket,const ulong deal_ticket,const ulong position_ticket)
   {
      int index=FindCommandIndex(order_ticket,deal_ticket,position_ticket);
      return index>=0 ? m_pending_command_ids[index] : "";
   }

};
