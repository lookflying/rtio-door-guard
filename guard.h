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
    ~Guard();

    void work();
    void refresh();
    void setRefreshPoint(int h, int m, int s = 0, int ms = 0);
    void setDistanceThreshold(int d);
    void setAction(GuardAction* action);
    void setError(int e);
private:
    CSerial m_port;
    bool m_working;
    TimeStamp m_refresh_point;
    int m_distance_threshold;
    int m_error;
    GuardAction* m_action;
    GuardLog m_log;
    int m_count;
    bool isTriggled(int distance);
};

#endif // GUARD_H
