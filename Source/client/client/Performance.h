#pragma once
#ifndef PERFORMANCE_H
#define PERFORMANCE_H
#include <cpprest/json.h>

static double overallCPUUsage = 0.0;
static int totalProcesses = 0;

/*
Returns total available physical memory in KB
@return The total available physical memory in system at a particular instance
*/
double availPhysicalMemory();

/*
Returns total used physical memory in KB
@return The total used physical memory in system at a particular instance
*/
double usedPhysicalMemory();

/*
Returns total physical memory used by a particular process in Bytes
@param hProcess Handle for the process whose memory consumption is to be determined.
@return The total used physical memory in by the process.
*/
long memoryForProcess(HANDLE hProcess);

/*
Get total no of processes running in a system at a particular instance.
@return Total no of processes
*/
int getTotalProcesses();

/*
@return Total CPU usage
*/
double getOverallCPUUsage();

/*
Returns a JSON value containing info about 
process name, it memory and CPU consumption.
@return A JSON value containing info of all processes
*/
web::json::value processList();

#endif
