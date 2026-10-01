//+------------------------------------------------------------------+
//|                                                 Protocol.mqh     |
//+------------------------------------------------------------------+
#property copyright "XAUUSD Sovereign"
#include "Common.mqh"

class CProtocol
{
public:
   static string BuildHandshake(const string timeframe)
   {
      return "{\"tf\":\"" + JsonEscape(timeframe) + "\",\"version\":1,\"token\":\"" + JsonEscape(InpAuthToken) + "\"}";
   }

   static string BuildTick(const string timeframe,const string symbol,const long event_time_msc,
                           const double bid,const double ask,const double last,
                           const double volume,const uint flags)
   {
      string json = "{";
      json += "\"tf\":\"" + JsonEscape(timeframe) + "\",";
      json += "\"sym\":\"" + JsonEscape(symbol) + "\",";
      json += "\"et\":" + UlongToStr((ulong)event_time_msc) + ",";
      json += "\"bid\":" + JsonNumber(bid,10) + ",";
      json += "\"ask\":" + JsonNumber(ask,10) + ",";
      json += "\"last\":" + JsonNumber(last,10) + ",";
      json += "\"vol\":" + JsonNumber(volume,2) + ",";
      json += "\"flg\":" + UintToStr(flags);
      json += "}";
      return json;
   }

   static string BuildBarClosed(const string timeframe,const string symbol,
                                const datetime open_time,const datetime close_time,
                                const double open,const double high,const double low,
                                const double close,const long tick_volume,const long real_volume)
   {
      string json = "{";
      json += "\"tf\":\"" + JsonEscape(timeframe) + "\",";
      json += "\"sym\":\"" + JsonEscape(symbol) + "\",";
      json += "\"ot\":" + UlongToStr((ulong)open_time * 1000) + ",";
      json += "\"ct\":" + UlongToStr((ulong)close_time * 1000) + ",";
      json += "\"o\":" + JsonNumber(open,10) + ",";
      json += "\"h\":" + JsonNumber(high,10) + ",";
      json += "\"l\":" + JsonNumber(low,10) + ",";
      json += "\"c\":" + JsonNumber(close,10) + ",";
      json += "\"tv\":" + IntegerToString(tick_volume) + ",";
      json += "\"rv\":" + IntegerToString(real_volume);
      json += "}";
      return json;
   }

   static string BuildHeartbeat(const string timeframe)
   {
      return "{\"tf\":\"" + JsonEscape(timeframe) + "\",\"ts\":" + UlongToStr((ulong)TimeCurrent()) + "}";
   }

   static string BuildSymbolSpec(const string broker,const string server,const string symbol,
                                 const int digits,const double point,const double tick_size,
                                 const double tick_value,const double contract_size,
                                 const double volume_min,const double volume_max,const double volume_step,
                                 const double stops_level,const double freeze_level)
   {
      string json = "{";
      json += "\"broker\":\"" + JsonEscape(broker) + "\",";
      json += "\"server\":\"" + JsonEscape(server) + "\",";
      json += "\"sym\":\"" + JsonEscape(symbol) + "\",";
      json += "\"digits\":" + IntegerToString(digits) + ",";
      json += "\"point\":" + JsonNumber(point,10) + ",";
      json += "\"tick_size\":" + JsonNumber(tick_size,10) + ",";
      json += "\"tick_value\":" + JsonNumber(tick_value,10) + ",";
      json += "\"contract_size\":" + JsonNumber(contract_size,4) + ",";
      json += "\"volume_min\":" + JsonNumber(volume_min,4) + ",";
      json += "\"volume_max\":" + JsonNumber(volume_max,4) + ",";
      json += "\"volume_step\":" + JsonNumber(volume_step,4) + ",";
      json += "\"stops_level\":" + JsonNumber(stops_level,4) + ",";
      json += "\"freeze_level\":" + JsonNumber(freeze_level,4);
      json += "}";
      return json;
   }

   static string BuildCommandResult(const string timeframe,const string command_id,const ulong ticket,
                                    const bool success,const string comment,const string stage="request",
                                    const int transaction_type=-1,const ulong order_ticket=0,
                                    const ulong deal_ticket=0,const ulong position_ticket=0,const uint retcode=0)
   {
      string json = "{";
      json += "\"tf\":\"" + JsonEscape(timeframe) + "\",";
      json += "\"cmd\":\"" + JsonEscape(command_id) + "\",";
      json += "\"ticket\":" + UlongToStr(ticket) + ",";
      json += "\"ok\":" + (success ? "true" : "false") + ",";
      json += "\"stage\":\"" + JsonEscape(stage) + "\",";
      json += "\"trans_type\":" + IntegerToString(transaction_type) + ",";
      json += "\"order\":" + UlongToStr(order_ticket) + ",";
      json += "\"deal\":" + UlongToStr(deal_ticket) + ",";
      json += "\"position\":" + UlongToStr(position_ticket) + ",";
      json += "\"retcode\":" + IntegerToString((long)retcode) + ",";
      json += "\"comment\":\"" + JsonEscape(comment) + "\"";
      json += "}";
      return json;
   }

   static string BuildTradeTransaction(const string timeframe,const string command_id,
                                       const bool success,const string comment,const int transaction_type,
                                       const ulong order_ticket,const ulong deal_ticket,
                                       const ulong position_ticket,const uint retcode)
   {
      return BuildCommandResult(timeframe,command_id,order_ticket!=0 ? order_ticket : deal_ticket,success,comment,
                                "transaction",transaction_type,order_ticket,deal_ticket,position_ticket,retcode);
   }

   static bool ExtractRaw(const string json,const string key,string &raw,bool &quoted)
   {
      string needle = "\"" + key + "\"";
      int start = StringFind(json,needle,0);
      if(start < 0) return false;
      int colon = StringFind(json,":",start + StringLen(needle));
      if(colon < 0) return false;
      int pos = colon + 1;
      while(pos < StringLen(json))
      {
         ushort ch = StringGetCharacter(json,pos);
         if(ch!=' ' && ch!='\t' && ch!='\r' && ch!='\n') break;
         ++pos;
      }
      if(pos >= StringLen(json)) return false;
      if(StringGetCharacter(json,pos)=='\"')
      {
         quoted = true;
         ++pos;
         int end = pos;
         bool escaped = false;
         while(end < StringLen(json))
         {
            ushort ch = StringGetCharacter(json,end);
            if(ch=='\"' && !escaped) break;
            if(ch=='\\' && !escaped) escaped = true;
            else escaped = false;
            ++end;
         }
         if(end >= StringLen(json)) return false;
         raw = StringSubstr(json,pos,end-pos);
         return true;
      }
      quoted = false;
      int end = pos;
      while(end < StringLen(json))
      {
         ushort ch = StringGetCharacter(json,end);
         if(ch==',' || ch=='}') break;
         ++end;
      }
      raw = StringTrimRight(StringSubstr(json,pos,end-pos));
      return true;
   }

   static string Unescape(const string value)
   {
      string out = value;
      StringReplace(out,"\\\"","\"");
      StringReplace(out,"\\n","\n");
      StringReplace(out,"\\r","\r");
      StringReplace(out,"\\t","\t");
      StringReplace(out,"\\\\","\\");
      return out;
   }

   static bool ExtractString(const string json,const string key,string &value)
   {
      bool quoted = false;
      if(!ExtractRaw(json,key,value,quoted) || !quoted) return false;
      value = Unescape(value);
      return true;
   }

   static bool ExtractDouble(const string json,const string key,double &value)
   {
      string raw;
      bool quoted = false;
      if(!ExtractRaw(json,key,raw,quoted) || quoted) return false;
      value = StringToDouble(raw);
      return true;
   }

   static bool ExtractUlong(const string json,const string key,ulong &value)
   {
      string raw;
      bool quoted = false;
      if(!ExtractRaw(json,key,raw,quoted) || quoted) return false;
      value = (ulong)StringToInteger(raw);
      return true;
   }
};
