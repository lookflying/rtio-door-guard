#include <cstdio>
#include <cstddef>
#include <cstring>

#include <sys/time.h>
#include <time.h>

#include "timestamp.h"


TimeStamp::TimeStamp(){
    getCurrentTime(this->m_ts, this->m_date);
}

TimeStamp::TimeStamp(int h, int m, int s, int ms){
    this->m_ts = (h * 3600 + m * 60 + s)*1000 + ms;
}

std::string TimeStamp::toString(){
    char c_file_str[50];
    sprintf(c_file_str,"%04d/%02d/%02d %02d:%02d:%02d",
            this->year(),
            this->month(),
            this->day(),
            this->hour(),
            this->minute(),
            this->second());
    return std::string(c_file_str);
}

std::string TimeStamp::toStringL(){
    char c_file_str[50];
    sprintf(c_file_str,"%04d/%02d/%02d %02d:%02d:%02d:%03d",
            this->year(),
            this->month(),
            this->day(),
            this->hour(),
            this->minute(),
            this->second(),
            this->millisecond());
    return std::string(c_file_str);
}


std::string TimeStamp::toFileName(){
    char c_file_str[20];
    sprintf(c_file_str,"%02d_%02d_%02d_%03d",
            this->hour(),
            this->minute(),
            this->second(),
            this->millisecond());
    return std::string(c_file_str);
}
void TimeStamp::getCurrentTime(int& ts, int& date){
    struct timeval tv;
    struct tm *tm;
    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    ts = 0;
    ts = (tm->tm_hour * 3600 + tm->tm_min * 60 + tm->tm_sec) *1000 + tv.tv_usec / 1000;
    date = (tm->tm_year + 1900)<<12 | (tm->tm_mon + 1) << 8 | tm->tm_mday;
}


