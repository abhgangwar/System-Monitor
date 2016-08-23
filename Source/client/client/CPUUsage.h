#pragma once
#ifndef CPUUSAGE_H
#define CPUUSAGE_H

#include <Windows.h>
#include <cpprest/json.h>

using namespace web::json;

/*
Class to calculate CPU usage of a particular process
*/
class CPUUsage
{
public:
	CPUUsage(HANDLE proc, value &obj);
	double findCurrentUsage();
private:
	ULARGE_INTEGER m_lastCPU, m_lastSysCPU, m_lastUserCPU;
	int m_numProcessors;
	HANDLE m_proc;
	value m_obj;
};
#endif // !CPUUSAGE_H