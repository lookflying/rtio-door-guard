/*
 * SysSeal.cpp
 *
 *  Created on: 2011-4-20
 *      Author: an
 */

#include "SysSeal.h"

unsigned	long o_cpu_user = 0;
unsigned	long o_cpu_sys = 0;
unsigned	long o_cpu_nice = 0;
unsigned	long o_cpu_idle = 0;

unsigned	long n_cpu_user =0 ;
unsigned	long n_cpu_sys = 0;
unsigned	long n_cpu_nice = 0;
unsigned	long n_cpu_idle = 0;

double GetCPUUsage()
{
	char tmp[64];
	FILE   *file   =   NULL;
	file   =   fopen( "/proc/stat","r");
	if(file   ==   NULL)
	{
		return -1;
	}
	fscanf(file,"%s   %lu   %lu   %lu   %lu ",tmp,&(n_cpu_user),&(n_cpu_sys),&(n_cpu_nice),&(n_cpu_idle));
	fclose(file);
	double t = (n_cpu_user - o_cpu_user )+ (n_cpu_sys - o_cpu_sys )+ (n_cpu_nice - o_cpu_nice )+ (n_cpu_idle - o_cpu_idle ) + 0.01;
	double idlev = (n_cpu_idle - o_cpu_idle );
	o_cpu_user= n_cpu_user;
	o_cpu_sys= n_cpu_sys;
	o_cpu_nice= n_cpu_nice;
	o_cpu_idle= n_cpu_idle;
	return (1.0 - idlev / t);
}
/****XSleep*************************************************************************************************/
void XSleep(long u_sec)
{
	struct timespec slptm;
	slptm.tv_sec = u_sec/1000;
	slptm.tv_nsec = (u_sec%1000)*1000000;      //1000000 ns = 1 us
	if(slptm.tv_nsec == 0)
	{
		slptm.tv_nsec = 1000000;
	}
	nanosleep(&slptm, NULL);;
}


/****xfstime*************************************************************************************************/

void GetXfsTime(xfstime * rettime)
{
	timeval tmp;
	gettimeofday(&tmp,NULL);
	* rettime = tmp.tv_sec * 1000000 + tmp.tv_usec;
}

long GetXfsTimePeriod(xfstime *timebegin,xfstime *timeend)
{
	xfstime ret = 0;
	ret = (*timeend - *timebegin);
	long ret2 = ret;
	return ret2;
}

static CSysTickCounter * systcntins = NULL;

CSysTickCounter::CSysTickCounter()
{
	systimertickconter = 0;
}
void SysTimerFunc(sigval_t a)
{
    systcntins->systimertickconter ++;
    int b = a.sival_int;//just for no warning
    a.sival_int = b;//just for no warning
}


long CSysTickCounter::GetSysTickCnt()
{
	if(systcntins == NULL)
	{
		systcntins = new CSysTickCounter;
		systcntins->systimertickconter = 0;
		systcntins->systickcnttimer.u_time_span = 100;
		systcntins->systickcnttimer.Func = (void *)SysTimerFunc;
		systcntins->systickcnttimer.StartTimer();
		return 0;
	}
	return systcntins->systimertickconter;
}
/******CXThread**********************************************************************************************/

CXThread::CXThread()
{
	th = 0;
}

CXThread::~CXThread()
{
}

void ThreadSWAPFunc(void *para)
{
	void ** p = (void **)para;
	void *(*FUNC)(void *);
	FUNC = (void *(*)(void *))(p[0]);
	FUNC(p[1]);
	CXThread * xth = (CXThread *)(p[2]);
	xth->th = 0;
}


bool CXThread::XCreateThread(void * func,void * para)
{

	if(func == NULL || th != 0)
	{
		return false;
	}
	//*/no used current
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	//*/
	int xthreadstate = pthread_create(&th,&attr,(void *(*)(void *))func,para);
	if(xthreadstate != 0)
	{
		th = 0;
		return false;
	}
	if(th != 0)
	{
		pthread_detach(th);
	}
	return true;
}
bool CXThread::XTerminateThread()
{
	if(th==0)
	{
		return true;
	}
	int ret = pthread_cancel(th);
	if(ret == 0)
	{
		return true;
	}
	return false;
}
bool CXThread::XThreadIsRunning()
{
	if(th != 0 )
	{
		return true;
	}
	return false;
}




/******CNetSever**********************************************************************************************/

CNetSever::CNetSever()
{
	net_data_agent = NULL;
	port = 0;
	data_size = 0;
	run_state = 0;
	close_target_socket = 0;
	clnaddr.s_addr = 0;
}

CNetSever::~CNetSever()
{

}



void CNetSever::SetNetDataAgent(CDataAgent * nda)
{
	net_data_agent = nda;
}


void CNetSever::OnDataRecv()
{
	net_data_agent->onData(this->data_buf,this->data_size);
}

bool CNetSever::InitNet()
{
	return true;
}

bool CNetSever::SetPort(int svr_port)
{
	if(svr_port<3000 )
	{
		return false;
	}
	port = svr_port;
	return true;
}

XSTDCALL NetSeverThread(void * para)
{
	if(para == NULL)
	{
		return ;
	}
	CNetSever * host = (CNetSever *)para;
	host->run_state = 1;

	while(host->run_state != 2)
	{
		XSleep(100);
	}
	if(1)
	{/////udp server
			int port = host->port;
			int fdmax;
			fd_set read_fds;
			fd_set master;
			//////////////////////////////////////////
			FD_ZERO(&master);

			SOCKET socket1;
			struct sockaddr from;
			struct sockaddr_in local;
			socklen_t fromlen =sizeof(struct sockaddr);
			local.sin_family=AF_INET;
			local.sin_port=htons(port);
			local.sin_addr.s_addr=INADDR_ANY;
			fromlen = sizeof(struct sockaddr);



			socket1=socket(AF_INET,SOCK_DGRAM,0);
			bind(socket1,(struct sockaddr*)&local,sizeof local);
			FD_SET(socket1,&master);
			fdmax=socket1;
			host->run_state = 8;

			while(1)
			{
				read_fds=master;
				if(select(fdmax+1,&read_fds,NULL,NULL,NULL)==-1)
				{
					host->run_state = 9;
					return;
				}

				if (FD_ISSET(socket1,&read_fds))
				{
					host->data_size = recvfrom(socket1,(char *)host->data_buf,MAXNETSVRBUFSIZE,0,&from,&fromlen);
					memcpy(&(host->clnaddr),&(from.sa_data[2]),sizeof(struct in_addr));
					if (host->data_size>0)
					{
						host->OnDataRecv();
					}
				}
			}
			return ;
	}

	return ;

}


bool CNetSever::StartSever()
{
	if(run_state != 0 || port == 0 || net_data_agent == NULL)
	{
		return false;
	}
	if(InitNet()==false)
	{
		return false;
	}
	//////////////////////////////////////////////////////////////

	if(thread_net_svr.XCreateThread((void *)NetSeverThread,this)==false)
	{
		return false;
	}

	//////////////////////////////////////////////////////////////
	while(run_state != 1)
	{////////////////////////////////wait  NetSeverThread

		XSleep(77);
	}
	run_state = 2;
	while((run_state != 8 ) && (run_state != 9))
	{
		XSleep(77);
	}

	if( run_state == 8)
	{//////ok result in NetSeverThread
		return true;
	}
	run_state =0;
	return false;

}



CNetClient::CNetClient()
{
	RunSocket = 0;
}

CNetClient::~CNetClient()
{
}

bool CNetClient::InitNet()
{
	return true;
}
bool CNetClient::OnSendData(void * data,int size)
{
	if(RunSocket <=0)
	{
		return false;
	}

	svlen =sizeof(pin);
	int ret = sendto(RunSocket,(char *)data,size,0,(struct sockaddr*)&pin,svlen);

	if(-1 != ret)
		return true;
	return false;
}
bool CNetClient::LinkSvr(char * ip,int Port)
{
	if(InitNet()==0)
	{
		return false;
	}

	if(Port<=0)
	{
		return false;
	}
	if(ip==NULL)
	{
		return false;
	}
	svlen =sizeof(pin);
	pin.sin_family=AF_INET;
	pin.sin_port=htons(Port);
	pin.sin_addr.s_addr=inet_addr(ip);
	RunSocket=socket(AF_INET,SOCK_DGRAM,0);
	if(RunSocket<=0)
	{
		return false;
	}
	return true;
}



/******CXTimer**********************************************************************************************/

CXTimer::CXTimer()
{
	Func = NULL;
	para = NULL;
	this->u_time_span = 0;
	Runtag = false;
}

CXTimer::~CXTimer()
{
}

bool CXTimer::StartTimer()
{
/////start timerthread func
	if(this->Func == NULL || this->Runtag == true || u_time_span <=0)
	{
		return false;
	}
	struct itimerspec timeout;
	sigset_t block;
	sigevent sev;
	sev.sigev_notify = SIGEV_THREAD;
	sev._sigev_un._sigev_thread._function = (void (*)(sigval_t))Func;
	sev._sigev_un._sigev_thread._attribute = NULL;
	sigemptyset(&block);
	sigaddset(&block, SIGALRM);
	sigprocmask(SIG_BLOCK, &block, NULL);

	if(timer_create(CLOCK_REALTIME, &sev, &thetimer) < 0)
	{
			return false;
	}
	timeout.it_interval.tv_sec = this->u_time_span / 1000;
	timeout.it_interval.tv_nsec = (this->u_time_span % 1000) * 1000000;
	timeout.it_value = timeout.it_interval;
	if(timer_settime(thetimer, 0, &timeout, NULL)<0)
	{
		return false;
	}
	Runtag = true;
	return true;
}

bool CXTimer::StopTimer()
{
	if(this->Runtag == false)
	{
		return true;
	}

	if(timer_delete(this->thetimer)<0)
	{
		return false;
	}
	Runtag = false;
	return true;
}

