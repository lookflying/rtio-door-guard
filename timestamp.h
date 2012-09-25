#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <string>
#include <cstdio>

class TimeStamp
{
public:
    TimeStamp();
    TimeStamp(int h, int m, int s = 0, int ms = 0);
    std::string toString();
    std::string toStringL();
    std::string toFileName();
    void operator = (TimeStamp x){
          this->m_ts = x.m_ts;
      }
      bool operator > (TimeStamp x){
          return this->m_ts > x.m_ts;
      }
      bool operator < (TimeStamp x){
          return this->m_ts < x.m_ts;
      }

      bool operator == (TimeStamp x){
          return this->m_ts == x.m_ts;
      }

      void operator = (int x){
          this->m_ts = x;
      }

      bool operator < (int x){
          return this->m_ts < x;
      }

      bool operator > (int x){
          return this->m_ts > x;
      }

      bool operator == (int x){
          return this->m_ts == x;
      }

      bool operator != (int x){
          return this->m_ts != x;
      }

      void set(int x){
          this->m_ts = x;
      }
      int year(){
          return (this->m_date >> 12);
      }

      int month(){
          return (this->m_date & 0x00000ff0)>>8;
      }

      int day(){
          return (this->m_date & 0x00000ff);
      }

      int hour(){
          return (int)((this->m_ts / 1000) / 3600 % 24);
      }

      int minute(){
          return (int)((this->m_ts/ 1000 / 60 ) % 60);
      }

      int second(){
          return (int)((this->m_ts / 1000) % 60);
      }

      int millisecond(){
          return (int)(this->m_ts % 1000);
      }


private:
    void getCurrentTime(int& ts, int&date);
    int m_ts;
    int m_date;


};

#endif // TIMESTAMP_H
