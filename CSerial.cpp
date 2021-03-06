/*
 * CSerial.cpp
 *
 *  Created on: 2011-4-21
 *      Author: an
 */

#include <cstdlib>

#include "CSerial.h"
CSerial::CSerial() {
    OpenSerielport = 0;
    m_distance = 0;
    sem_init(&this->m_mutex, 0, 1);
}

CSerial::~CSerial() {
    CloseSerial();
}


void CSerial::SendData(unsigned char * data,int size)
{
	long pos = 0;
	int nByte = write(OpenSerielport, & data[pos],size - pos);
	pos += nByte;
	while(pos < size)
	{
		nByte = write(OpenSerielport, & data[pos],size - pos);
		pos += nByte;
	}
	return;
}

bool CSerial::CloseSerial()
{

	if(OpenSerielport == 0)
	{
		return true;
	}
	if(1)
	{
		thread.XTerminateThread();
		XSleep(100);
	}
	this->StopIns();
	close(OpenSerielport);
	OpenSerielport = 0;
	return true;
}
bool CSerial::OpenSerial(const char * serialtag,int serial_num,int baudrate , int databits,int stopbits,int parity)
{
	char name[50];
	memset(name,'\0',50);
	if(serialtag != NULL)
	{
		sprintf(name,"/dev/%s%d",serialtag,serial_num);
	}
	else
	{
		sprintf(name,"/dev/ttyS%d",serial_num);
	}

	OpenSerielport = open(name,O_RDWR);
//	printf("OpenSerielport %d\n",OpenSerielport);
	if(OpenSerielport <=0 )
	{
		OpenSerielport = 0;
		return false;
	}

	if(1)
	{
		int fd = OpenSerielport;
		speed_t speed;
		int   status;
		struct termios Opt;
		tcgetattr (fd, &Opt);
			switch (baudrate)
				{
				case 110:
					speed=B110;
					break;
				case 300:
					speed=B300;
					break;
				case 600:
					speed=B600;
					break;
				case 1200:
					speed=B1200;
					break;
				case 2400:
					speed=B2400;
					break;
				case 4800:
					speed=B4800;
					break;
				case 9600:
					speed=B9600;
					break;
				case 19200:
					speed=B19200;
					break;
				case 38400:
					speed=B38400;
					break;
				case 57600:
					speed=B57600;
					break;
				case 115200:
					speed=B115200;
					break;
				default:
					close(OpenSerielport);
					OpenSerielport = 0;
					return false;
					break;
				}
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed);
			cfsetospeed(&Opt, speed);
			status = tcsetattr(fd, TCSANOW, &Opt);
			if  (status != 0)
			{
				close(OpenSerielport);
				OpenSerielport = 0;
				return false;
			}
			tcflush(fd,TCIOFLUSH);
	}

	if(1)
	{
		int fd = OpenSerielport;
			struct termios options;
		 	if(tcgetattr(fd,&options)!= 0)
		 	{
		 		close(OpenSerielport);
				OpenSerielport = 0;
		 		return false;
		 	}
		 	options.c_cflag &= ~CSIZE;
		 	switch (databits) /*设置数据位数*/
		 	{
		 	case 7:
		 		options.c_cflag |= CS7;
		 		break;
		 	case 8:
		 		options.c_cflag |= CS8;

		 		break;
		 	default:
		 		close(OpenSerielport);
				OpenSerielport = 0;
		      return false;
		 	}
		 switch (parity)
		 {
		 	case 1:
		 		//options.c_cflag &= ~PARENB;   /* Clear parity enable */
		 		//options.c_iflag &= ~INPCK;     /* Enable parity checking */
				options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
				options.c_oflag &= ~OPOST; /*Output*/
		 		//options.c_iflag &= ~( ICRNL | IXON); /*Input*/

		 		options.c_iflag &= ~( ICRNL | IXON); /*Input*/

				//options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
				//options.c_oflag &= ~OPOST; /*Output*/
		 		break;

		 	case 2:
		 		options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
		 		options.c_iflag |= INPCK;             /* Disnable parity checking */
		 		break;
		 	case 3:
		 		options.c_cflag |= PARENB;     /* Enable parity */
		 		options.c_cflag &= ~PARODD;   /* 转换为偶效验*/
		 		options.c_iflag |= INPCK;       /* Disnable parity checking */
		 		break;
		 	default:
		 		close(OpenSerielport);
				OpenSerielport = 0;
		 		return false;
		 	}
		 /* 设置停止位*/
		 switch (stopbits)
		 {
		 	case 1:
		 		options.c_cflag &= ~CSTOPB;
		 		break;
		 	case 2:
		 		options.c_cflag |= CSTOPB;
		 	   break;
		 	default:
		 		close(OpenSerielport);
				OpenSerielport = 0;
		 		 return false;
		 }
		 /* Set input parity option */
		 options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
		 		options.c_oflag &= ~OPOST; /*Output*/
		 if (parity != 1)
		 	options.c_iflag |= INPCK;
		 tcflush(fd,TCIFLUSH);
		 options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/
		 options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
		 if (tcsetattr(fd,TCSANOW,&options) != 0)
		 {
		 		close(OpenSerielport);
				OpenSerielport = 0;
				return false;
		 }
	}
	return true;
}


long serialfunc(void *p)
{

	CSerial * serial = (CSerial *)p;
	fd_set rdfds; // 先申明一个fd_set集合来保存要检测的串口句柄
	struct timeval timeout = { 30, 10 }; // 非阻塞方式
	int judge;
	unsigned char redc;
	int nread;

	while(1)
	{

		int maxfds = 0;
		FD_ZERO(&rdfds); //用select函数之前先把集合清零
		if(serial->OpenSerielport >0 )
		{
			FD_SET(serial->OpenSerielport,&rdfds);
			maxfds = serial->OpenSerielport;
		}
		else
		{
			return 0;
		}

		timeout.tv_sec =  30;
		timeout.tv_usec =  30;
		judge = select(maxfds+1, &rdfds, NULL, NULL, &timeout);
		//printf("judge %d,seriel %d,maxfds %d\n",judge,ls->serials[0].OpenSerielport,maxfds);

		switch (judge)
		{
			case -1:
				break;
			case 0:
				break;
			default:
				if (FD_ISSET(serial->OpenSerielport,&rdfds))
				{
						nread = read(serial->OpenSerielport, &redc, 1);
						if(nread >= 1)
						{
							serial->OnData(redc);
						}
				}
				break;
		}
	}
	return 0;
}

bool CSerial::StartSerial()
{
	if(OpenSerielport == 0 )
	{
		return false;
	}
	this->StartIns();
	return thread.XCreateThread((void *)serialfunc,(void *)this);

}





void CSerial:: OnData(unsigned char cbuf){
    int distance = -1;
    if(onGetDistance(distance, cbuf)){
        if(sem_trywait(&this->m_mutex)){
            this->m_distance = distance;
            sem_post(&this->m_mutex);
        }
    }

}

bool CSerial::onGetDistance(int &distance, unsigned char cbuf){
    static char buf[20];
    static int offset = 0;
//    printf("%c %d\n",cbuf, offset);
    if(cbuf == 'R'){
        offset =0;
    }else if(cbuf <= '9' && cbuf >= '0'){
        buf[offset] = cbuf;
        offset ++;
        if(offset == 3){
            buf[offset] = '\0';
            distance = atoi(buf);
            return true;
        }
    }
    return false;
}

int CSerial::getDistance(){
    sem_wait(&this->m_mutex);
    return this->m_distance;
    sem_post(&this->m_mutex);
}

