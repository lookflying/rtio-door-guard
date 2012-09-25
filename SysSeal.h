/*
 * SysSeal.h
 *
 *  Created on: 2011-4-20
 *      Author: an
 */

#ifndef SYSSEAL_H_
#define SYSSEAL_H_

#include	<stdio.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include	<sys/types.h>
#include	<sys/time.h>
#include	<sys/times.h>
#include	<sys/ioctl.h>
#include	<unistd.h>
#include	<pthread.h>
#include	<fcntl.h>

#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/stat.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sched.h>
#include <stdarg.h>

/***************************************************************************************************/
#define SOCKET int
#define XSTDCALL void
#define xfstime long long
#define MAXNETSVRBUFSIZE 1024
/***************************************************************************************************/
/*
 * this is the call for sleep
 * the parameter u_sec indicate the sleep period
 *  (millisecond)
 */
void XSleep(long u_sec);

/*
 *  GetXfsTime get current xfstime;
 *  xfstime can't be used anywhere except  GetXfsTimePirod
 */
void GetXfsTime(xfstime * rettime);
/*
 * GetXfsTimePeriod get time period in 1/1000000 second
 */
long GetXfsTimePeriod(xfstime *timebegin,xfstime *timeend);


double GetCPUUsage();





/***************************************************************************************************/
/*
 * this is the class for thread
 * the thread func must be the form bellow(both in windows and linux ):
 * XSTDCALL func(void * para);
 */
class CXThread
{
public:
	CXThread();
	~CXThread();
	bool XCreateThread(void * func,void * para);
	bool XTerminateThread();
	bool XThreadIsRunning();
	pthread_t th;
protected:
};





class CDataAgent
{
public:
	CDataAgent(){};
	virtual ~CDataAgent(){};
	virtual void onData(char * data_buf,long data_size) = 0;
};


class CNetSever
{
public:
	CNetSever();
	virtual ~CNetSever();
	bool SetPort(int svr_port);
	bool StartSever();
	void SetNetDataAgent(CDataAgent * nda);
	bool InitNet();
	struct in_addr clnaddr;
protected:
	void OnDataRecv();
	CDataAgent * net_data_agent;
	int port;
	char data_buf[MAXNETSVRBUFSIZE];
	long data_size;

	friend XSTDCALL NetSeverThread(void * para);
	char run_state;
	socklen_t close_target_socket;
	CXThread  thread_net_svr;
};

class CNetClient
{
public:
	CNetClient();
	virtual ~CNetClient();
	virtual bool OnSendData(void * data,int size);
	virtual bool LinkSvr(char * ip,int Port);
protected:
	int RunSocket;
	bool InitNet();
	struct sockaddr_in pin;
	int svlen;
};

/*
 * timer class
 * in linux the Func must be the form bellow:
 * void Func(sigval_t a)
 */
class CXTimer
{
public:
	CXTimer();
	virtual ~CXTimer();
	bool StartTimer();
	bool StopTimer();
	void * Func;
	void * para;
	int u_time_span;
	timer_t thetimer;
	bool Runtag;
};


class CSysTickCounter
{
public:
	virtual ~CSysTickCounter(){};
	static long GetSysTickCnt();
	long systimertickconter;
	CXTimer systickcnttimer;
	CSysTickCounter();
};

#endif /* SYSSEAL_H_ */
