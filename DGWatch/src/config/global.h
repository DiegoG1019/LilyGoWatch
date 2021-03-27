#ifndef GLOBAL_H
#define GLOBAL_H

#include "config.h"
#include <Arduino.h>
#include <LilyGoWatch.h>
#include <arduino-timer.h>
#include "Log.h"

TTGOClass* ttgo;
Timer<TIMER_TASKS>* timer;

constexpr auto Filename = __FILE__;

bool LoadSystemStatus(){
	Log::Information("Attempting to load System_Status data from Flash Memory", Filename, __LINE__);
	uint32_t* addrs = reinterpret_cast<uint32_t*>(&System_Status);
	ESP.flashRead(reinterpret_cast<uintptr_t>(&System_Status_FLASH), addrs, sizeof(System_Status_t));
	Log::Information(System_Status.GetFormatted(), Filename, __LINE__);
	bool csd = System_Status.CorrectlyShutDown;
	System_Status.CorrectlyShutDown = false;
	return csd;
}

void SaveSystemStatus() {
	Log::Information("Attempting to save System_Status data to Flash Memory", Filename, __LINE__);
	if (!System_Status.Edited) {
		Log::Information("System_Settings has not been edited, no need to save.", Filename, __LINE__);
		return;
	}
	System_Status.CorrectlyShutDown = true;
	ESP.flashWrite(reinterpret_cast<uintptr_t>(&System_Status_FLASH), reinterpret_cast<uint32_t*>(&System_Status), sizeof(System_Status_t));
	Log::Information("Succesfully saved System_Status data to Flash Memory", Filename, __LINE__);
}

void DeepSleep() {
	SaveSystemStatus();
	ESP.deepSleep(UINT32_MAX);
};

inline int MemoryUnits(int bytes, int unit = 1) {
    return bytes / (1024 * unit);
}

inline void Fail(){
    Log::ForceFlush();
    ESP.restart();
}

#endif