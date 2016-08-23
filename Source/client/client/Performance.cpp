#include <string>
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>

#include "Performance.h"
#include "CPUUsage.h"

using namespace web::json;

double availPhysicalMemory()
{
	MEMORYSTATUSEX mInfo;
	mInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&mInfo);
	return static_cast<double>(mInfo.ullAvailPhys/1024);
}

double usedPhysicalMemory()
{
	MEMORYSTATUSEX mInfo;
	mInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&mInfo);
	std::cout << "total: " << static_cast<double>(mInfo.ullTotalPhys/1024)
		<< " used: " << static_cast<double>(mInfo.ullAvailPhys/1024)
		<< "\nDiff: " << ((static_cast<double>(mInfo.ullTotalPhys) - static_cast<double>(mInfo.ullAvailPhys)))/1024 << std::endl;
	return ((static_cast<double>(mInfo.ullTotalPhys) - static_cast<double>(mInfo.ullAvailPhys))/1024);
}

value processList()
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Fetching a snapshot of all processes
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		std::cout << "CreateToolhelp32Snapshot (of processes)" << std::endl;
		return(value());
	}

	//  Setting size of structure before using it is mendatory
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Checking info about the process
	if (!Process32First(hProcessSnap, &pe32))
	{
		std::cout << " Unable to fetch info of first process using Process32First" << std::endl;
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(value());	// Return a NULL value
	}

	// Fetching info about all the processes
	value arr;
	std::vector<CPUUsage> cpus;
	int cnt = 0;
	do
	{
		value obj;
		++totalProcesses;
		obj[U("processName")] = value::string(pe32.szExeFile);
		HANDLE hProcess;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		if (hProcess == NULL)
		{
			std::cout << "Unable to open process. PID: " << pe32.th32ProcessID << std::endl;
		}
		else {
			obj[U("memoryConsumed")] = value::number(memoryForProcess(hProcess));
			arr[cnt] = obj;
			CPUUsage cpu(hProcess, arr[cnt]);
			cpus.push_back(cpu);
			++cnt;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	// Wait for 1 second so that we can calculate CPU of process.
	Sleep(1000);

	array v = arr.as_array();
	for (size_t i = 0; i < cpus.size(); ++i)
	{
		printf(" For i: %d  ", i);
		double usage = cpus[i].findCurrentUsage();
		overallCPUUsage += usage;
		v[i][U("cpuUsage")] = value::number(usage);
		std::cout << " Process Name: " << utility::conversions::to_utf8string( (v[i][U("processName")].as_string()) ) << " ";
		printf(" memoryConsumed: %d ", (v[i][U("memoryConsumed")].as_number()));
		printf(" cpuUsage: %f\n ", (v[i][U("cpuUsage")].as_number()));
	}

	return arr;
}

long memoryForProcess(HANDLE hProcess)
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		return pmc.WorkingSetSize;
	}
	else
		return -1;
}

double getOverallCPUUsage()
{
	return overallCPUUsage;
}

int getTotalProcesses()
{
	return totalProcesses;
}