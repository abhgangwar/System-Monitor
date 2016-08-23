#include <iostream>
#include <fstream>
#include "Server.h"

std::ofstream fout;
std::ifstream fin;

Server::Server(string_t baseUrl, const char* email, const char* pass) : 
	m_listener(baseUrl),
	m_smtpClient(email, pass),
	m_xml("clients.xml")
{
	m_listener.support(methods::POST, std::bind(&Server::handlePostReq, this, std::placeholders::_1));
	m_listener.support(methods::GET, std::bind(&Server::handlePostReq, this, std::placeholders::_1));
}

ClientData Server::getClientData(std::string key)
{
	return m_xml.getClientData(key);
}

void Server::handlePostReq(http_request req)
{
	std::cout << "Request received" << std::endl;
	req.extract_json().then([=](value body)
	{
		if (body == value()) {
			std::cout << "Empty request body" << std::endl;
			req.reply(status_codes::BadRequest, U("Request body was empty")).then([](pplx::task<void> t) { handle_error(t); });
			return;
		}
		object jsonData = body.as_object();
		if (jsonData.empty())
		{
			std::cout << "Empty request body" << std::endl;
			req.reply(status_codes::BadRequest, U("Request body was empty")).then([](pplx::task<void> t) { handle_error(t); });
			return;
		}
		std::string user = conversions::to_utf8string(jsonData[U("user")].as_string());
		std::string pass = conversions::to_utf8string(jsonData[U("pass")].as_string());

		//std::cout << "Determined key: " << user << std::endl;
		//std::cout << "Determined key: " << pass << std::endl;

		value processes = jsonData[U("processes")];
		double cpu = jsonData[U("overallCPUUsage")].as_double();
		double availMem = jsonData[U("overallAvailMem")].as_double();
		double usedMem = jsonData[U("overallUsedMem")].as_double();
		int noProcesses = jsonData[U("totalProcesses")].as_integer();
		double percentMemUse = (usedMem * 100) / (usedMem + availMem);

		//std::cout << "cpu: " << cpu << std::endl;
		//std::cout << "a: " << availMem << std::endl;
		//std::cout << "m: " << usedMem << std::endl;
		//std::cout << "%cen: " << percentMemUse << std::endl;
		//std::cout << "nP: " << noProcesses << std::endl;

		ClientData d = getClientData(user + pass);
		if (d.mail == "") {
			std::cout << "Wrong username/password" << std::endl;
			req.reply(status_codes::Unauthorized, U("Wrong User/password")).then([](pplx::task<void> t) { handle_error(t); });
			return;
		}
		else
		{
			std::string userEmail = d.mail;
			std::map<std::string, int> limits = d.limits;
			std::string emailText = "Hi,\nYou have violated given limit for following criteria: \n";
			bool flag = false;

			if (percentMemUse > limits["memory"])
			{
				emailText += "Total Memory Usage\n";
				flag = true;
			}

			if (cpu > limits["cpu"])
			{
				emailText += "Total CPU Usage\n";
				flag = true;
			}

			if (noProcesses > limits["processes"])
			{
				emailText += "Total no of processes that can be run.\n\n";
				flag = true;
			}

			emailText += "\n\nThanks";

			std::string logText = conversions::to_utf8string(body.serialize());
			std::cout << "Got body: " << logText << std::endl;
			fout.open("logs.txt", std::ios::app);	// logging the data on disk.
			if (!fout) {
				std::cout << "Error while opening file logs.txt" << std::endl;
			}
			fout << "//////////////////// Data Entry Begin  ////////////////////" << std::endl << std::endl;
			fout << logText << std::endl << std::endl;
			fout << "//////////////////// Data Entry End  ////////////////////" << std::endl;
			fout.close();

			req.reply(status_codes::OK, U("Replying")).then([](pplx::task<void> t) { handle_error(t); });
			if (flag)
			{
				std::cout << "Sending email: " << std::endl;
				const char *to = userEmail.c_str();
				char * sub = "Alert for violation of limted sytem usage";
				char *from = "CPP Cloud Service";
				char *content = &emailText[0];
				m_smtpClient.sendEmail(from, to, sub, content);
			}
		}
	});
}

void Server::handle_error(pplx::task<void>& t)
{
	try
	{
		t.get();
	}
	catch(...)
	{
		std::cout << "Error encountered" << std::endl;
	}
}

pplx::task<void> Server::open()
{
	
	return m_listener.open().then([] (pplx::task<void> t)
	{
		handle_error(t);
	});
	
	//return m_listener.open.wait();
}

pplx::task<void> Server::close()
{
	return m_listener.close().then([](pplx::task<void> t)
	{
		handle_error(t);
	});
}

int main(int argc, char **argv)
{
	fin.open("config.txt", std::ios::in);
	if (!fin)
	{
		std::cout << "Unable to open config file config.txt";
		return 0;
	}
	std::string email;
	std::string pass;
	std::string addr;

	fin >> email >> email;
	fin >> pass >> pass;
	fin >> addr >> addr;
	fin.close();
	Server s(conversions::to_string_t(addr), email.c_str(), pass.c_str());
	s.open().wait();
	int a;
	std::cin >> a;
	s.close().wait();
	return 0;
}