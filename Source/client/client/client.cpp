// client.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <fstream>
#include <cpprest/json.h>
#include <cpprest/http_client.h>
#include <pplx/pplxtasks.h>
#include "Performance.h"
#include "CPUUsage.h"

using namespace web::json;					// For JSON
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

std::ifstream fin;

int main()
{
	fin.open("config.txt", std::ios::in);
	if (!fin) {
		std::cout << "Unable to open config file" << std::endl;
		return 0;
	}
	std::string user; 
	std::string pass;
	std::string addr;
	fin >> user >> user;
	fin >> pass >> pass;
	fin >> addr >> addr;
	fin.close();

	// Collect all the data to be sent to service.
	value processes =  processList();
	value data;
	data[U("user")] = value::string(conversions::to_string_t(user));
	data[U("pass")] = value::string(conversions::to_string_t(pass));
	data[U("overallCPUUsage")] = value::number(getOverallCPUUsage());
	data[U("overallAvailMem")] = value::number(availPhysicalMemory());
	data[U("overallUsedMem")] = value::number(usedPhysicalMemory());
	data[U("totalProcesses")] = value::number(getTotalProcesses());
	data[U("processes")] = processes;
	//std::cout << "overall cpu: " << getOverallCPUUsage() << std::endl;
	//std::cout << "avail mem: " << availPhysicalMemory() << std::endl;
	//std::cout << "used mem: " << usedPhysicalMemory() << std::endl;

	string_t str = conversions::to_string_t(addr);
	http_client client(str);
	uri_builder path(U("/"));
	pplx::task< void > reqTask = client.request(methods::POST, path.to_string(), data)
		.then([=](http_response response)
	{
		printf("Received response status code:%u\n", response.status_code());
	});
	try
	{
		reqTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}
	return 0;
}
