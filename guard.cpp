#include "guard.h"
#include "cmath"
#define DEBUG 0
Guard::Guard(const char* port_tag, int num)
{
    this->m_action = NULL;
    this->m_working = false;
    this->m_count = 0;
    this->m_error = 3;
    this->m_down_threshold = 5;
    if(!this->m_port.OpenSerial(port_tag, num, 9600, 8, 1, 1)){
        perror("Can't open serial port");
        return;
    }
    this->m_working = true;
    printf("Serial Port Opened!!\n");

}
Guard::~Guard(){
    printf("Guard die...\n");
}

void Guard::work(){
    this->m_port.StartSerial();
    while(this->m_working){
        TimeStamp ts;
        int cur_distance = this->m_port.getDistance();
        if (isTriggled(cur_distance)){
            if(this->m_action != NULL){
                if((*this->m_action)(cur_distance, ts)){
                    ++this->m_count;
                }
            }
        }
#if DEBUG
        this->m_log.write(ts, cur_distance, this->m_count);
#endif
    }
}

void Guard::refresh(){
    this->m_count = 0;
}

void Guard::setRefreshPoint(int h, int m, int s, int ms){
    this->m_refresh_point = TimeStamp(h, m, s, ms);
}

void Guard::setDistanceThreshold(int d){
    this->m_distance_threshold = d;
}

void Guard::setAction(GuardAction* action){
    this->m_action = action;
}

bool Guard::isTriggled(int distance){
	static int old_dis = 0;
    static int down_count = 0;
    static int up_count = 0;
	if (std::abs(old_dis - distance) > this->m_error){
		old_dis = distance;
		if(distance < this->m_distance_threshold){
            if (distance <= old_dis){
                down_count ++;
            }else{
                down_count = 0;
            }
            if (down_count > this->m_down_threshold){
                down_count = 0;
                return true;
            }
        }
	}
	return false;
}

void Guard::setError(int e){
    this->m_error = e;
}

void Guard::setDownThreshold(int t){
    this->m_down_threshold = t;
}
