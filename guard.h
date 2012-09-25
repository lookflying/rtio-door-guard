#ifndef GUARD_H
#define GUARD_H
#include "CSerial.h"
#include "timestamp.h"
#include "guardlog.h"
#include <cstddef>
typedef bool GuardAction(int distance, TimeStamp ts);
class Guard
{
public:
    Guard(const char* port_tag = "ttyUSB", int num = 0);
    ~Guard(){
        printf("killed?\n");
    }

    void work();
    void refresh();
    void setRefreshPoint(int h, int m, int s = 0, int ms = 0);
    void setDistanceThreshold(int d);
    void setAction(GuardAction* action);
private:
    CSerial m_port;
    bool m_working;
    TimeStamp m_refresh_point;
    int m_distance_threshold;
    GuardAction* m_action;
    GuardLog m_log;
    int m_count;
};

#endif // GUARD_H
