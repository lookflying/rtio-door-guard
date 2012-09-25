#include "guardlog.h"
#include <time.h>

GuardLog::GuardLog()
{
    this->m_file_is_open =false;
    newLogFile();
}
GuardLog::~GuardLog(){
    if(this->m_file_is_open){
        fclose(this->m_file);
    }
}

std::string GuardLog::getFileName(){
    char buf[30];
    time_t t = time(NULL);
    struct tm *tt = localtime(&t);
    sprintf(buf, "%04d_%02d_%02d__%02d_%02d_%02d",
            tt->tm_year + 1900,
            tt->tm_mon + 1,
            tt->tm_mday,
            tt->tm_hour,
            tt->tm_min,
            tt->tm_sec);
    return std::string(buf);
}

void GuardLog::write(TimeStamp ts, int d, int c){
    static int old_d = -1;
    if(this->m_file_is_open){
        if(old_d != d){
            fprintf(this->m_file, "%s\t%4d cm %5d\n", ts.toString().c_str(), d, c);
            fflush(this->m_file);
            old_d = d;
        }
    }else{
        fprintf(stderr, "file is not open while writing log");
    }
}

void GuardLog::newLogFile(){
    if(this->m_file_is_open){
        fclose(this->m_file);
        this->m_file_is_open = false;
    }
    this->m_cur_file = getFileName() + ".txt";
    this->m_file = fopen(this->m_cur_file.c_str(), "a");
    if(this->m_file == NULL){
        perror("Can't open file in guard log");
        return;
    }
    this->m_file_is_open = true;
}
