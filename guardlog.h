#ifndef GUARDLOG_H
#define GUARDLOG_H
#include <string>
#include <cstdio>
#include "timestamp.h"

class GuardLog
{
public:
    GuardLog();
    ~GuardLog();
    void write(TimeStamp ts, int d, int c);
    void newLogFile();
private:
    std::string getFileName();
    std::string m_cur_file;
    FILE* m_file;
    bool m_file_is_open;

};

#endif // GUARDLOG_H
