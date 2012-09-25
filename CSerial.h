/*
 * CSerial.h
 *
 *  Created on: 2011-4-21
 *      Author: an
 */

#ifndef CSERIAL_H_
#define CSERIAL_H_

#include "SysSeal.h"
#include     <termios.h>    /*PPSIX 终端控制定义*/
#include <semaphore.h>
class CSerial {
private:
    int m_distance;
public:
	CSerial();
	virtual ~CSerial();
    bool OpenSerial(const char * serialtag,int serial_num,int baudrate , int databits,int stopbits,int parity);
    virtual void StartIns(){};
    virtual void StopIns(){};
	bool CloseSerial();
	void SendData(unsigned char * data,int size);
	bool StartSerial();
    virtual void OnData(unsigned char cbuf);
    bool onGetDistance(int& distance, unsigned char cbuf);

	CXThread thread;
	int OpenSerielport;
    sem_t m_mutex;
    int getDistance();
};

#endif /* CSERIAL_H_ */
