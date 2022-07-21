#include "sys/types.h"
#include "sys/sysinfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using std::cerr;
using std::endl;
#include <fstream>
using std::ofstream;
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <unistd.h>

static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void init(){
    FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
        &lastTotalSys, &lastTotalIdle);
    fclose(file);
}

double getCurrentValue(){
	
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
        &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
            (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;
}


int main(){
	init();
	ofstream outdata;
	outdata.open("../system.csv");
	if( !outdata){
		cerr << "Error FILE" << endl;
	}
	outdata << "Time,CPU used, RAM used, VRAM used" << std::endl;
	while (true){
		int sysinfo(struct sysinfo *info);

		std::cout << info.totalram * info.mem_unit;
		
		std::cout << info.totalswap * info.mem_unit;

		unsigned long physMemUsed = info.totalram * info.mem_unit - info.freeram * info.mem_unit;

		unsigned long virtualMemUsed = info.totalswap * info.mem_unit - info.freeswap * info.mem_unit;
		
		double cpuused = getCurrentValue();
	
	    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	    std::string snow( ctime( &now ) );
	    snow = snow.substr(0,snow.length()-1);
	    outdata << snow << "," << std::to_string(cpuused) << "," << std::to_string(physMemUsed) << "," << std::to_string(virtualMemUsed) << std::endl;
	    
	    sleep(1);
	}
}





