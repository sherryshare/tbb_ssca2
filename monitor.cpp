#ifndef MONITOR_CPP
#define MONITOR_CPP
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<cmath>
#include "monitor.h"
using namespace std;
double cpuInfo[TOTAL_CORE];
void getCpuInfo()
{
    float nice[2][TOTAL_CORE + 2];
    float user[2][TOTAL_CORE + 2];
    float system[2][TOTAL_CORE + 2];
    float idle[2][TOTAL_CORE + 2];
    float total[2][TOTAL_CORE + 2];
    float addtional[2][TOTAL_CORE + 2];
	char _cpu[21];
    float _user;
    float _nice;
    float _system;
    float _idle;
    float _total;
    float _addtional;
    char text[401];
	int cnt = 0;
	int tcnt = 0;
	FILE *fp;
	fp = fopen("/proc/stat", "r");
	while (fgets(text, 200, fp))
	{
		if (strstr(text, "cpu"))
		{
			sscanf(text, "%s %f %f %f %f %f", _cpu, &_user, &_nice, &_system, &_idle, &_addtional);
			user[0][tcnt] = _user;
			nice[0][tcnt] = _nice;
			system[0][tcnt] = _system;
			idle[0][tcnt] = _idle;
			addtional[0][tcnt] = _addtional;
			tcnt++;
			//printf("first time %f %f %f %f %f\n",_user,_nice,_system,_idle,_addtional);
		}
		if(tcnt == TOTAL_CORE + 1)
			break;
	}
	fclose(fp);
	usleep(500000);
	tcnt = 0;
	fp = fopen("/proc/stat", "r");
	while (fgets(text, 200, fp))
	{
		if (strstr(text, "cpu")){
			sscanf(text, "%s %f %f %f %f %f", _cpu, &_user, &_nice, &_system, &_idle, &_addtional);
			user[1][tcnt] = _user;
			nice[1][tcnt] = _nice;
			system[1][tcnt] = _system;
			idle[1][tcnt] = _idle;
			addtional[1][tcnt] = _addtional;
			tcnt++;
			//printf("second time %f %f %f %f %f\n",_user,_nice,_system,_idle,_addtional);
		}
		if(tcnt == TOTAL_CORE + 1)
			break;
	}
	for(int i = 1; i < TOTAL_CORE + 1; ++i)
	{
		float total = (user[1][i] - user[0][i]) + (nice[1][i] - nice[0][i]) + (system[1][i] - system[0][i]) + (idle[1][i] - idle[0][i]) + (addtional[1][i] - addtional[0][i]);
		float use = 0;
		if (total != 0)
		 use = round((user[1][i] - user[0][i]) / total * 100);
                //printf("total=%f,fen bie wei %f  %f  %f  %f  %f\n",total,(user[1][i] - user[0][i]),(nice[1][i] - nice[0][i]), (system[1][i] - system[0][i]),(idle[1][i] - idle[0][i]),(addtional[1][i] - addtional[0][i]));
		cpuInfo[i-1] = use ;
		//user[0][i] = user[1][i];
		//nice[0][i] = nice[1][i];
		//system[0][i] = system[1][i];
		//idle[0][i] = idle[1][i];
		//addtional[0][i] = addtional[1][i];
	}
	fclose(fp);
}

#endif
