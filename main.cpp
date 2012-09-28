#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
#include "CSerial.h"
#include "guard.h"
#include "timestamp.h"

const char* WWW_ROOT = "/dev/shm/www/index.html";

bool guardAction(int distance, TimeStamp ts){
	FILE* htmlp = fopen(WWW_ROOT, "w");
	VideoCapture cap;
	Mat img;
	if(cap.open(-1)){
		cap >> img;
		imwrite("/dev/shm/www/test.jpg", img);
	}
	if(0 == htmlp)
	{
		return false;
	}

    fprintf(htmlp,"<html><head><title>Door Guard 1.0</title></head><body><h1>[%s]Door was open. </h1></body></html>",
			ts.toString().c_str());

	fclose(htmlp);
	
    printf("%s %d cm\n", ts.toString().c_str(), distance);
	
	return true;
}

int main()
{
    Guard g;
    g.setAction(&guardAction);
    g.setDistanceThreshold(70);
    TimeStamp ts;
    FILE * htmlp = fopen(WWW_ROOT, "w");
    fprintf(htmlp, "<html><head><title>Door Guard 1.0</title></head><body><h1>[%s]Door Guard start !</h1></body></html>", ts.toString().c_str());
    fclose(htmlp);
    printf("Guard start to work!\n");
    g.work();
}



