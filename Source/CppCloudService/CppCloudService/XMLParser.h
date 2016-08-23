#pragma once
#include "Markup.h"
#include <map>

/*
Class to store data of a client. 
*/
class ClientData
{
public:
	ClientData()
	{
		user = "";
		pass = "";
		mail = "";
	}

	ClientData(std::string user, std::string pass, std::string mail, std::map<std::string, int> limits) :
		user(user),
		pass(pass),
		mail(mail),
		limits(limits)
	{
		// Other code if required.
	}

	// Copy constructor;
	ClientData(const ClientData &other)
	{
		this->user = other.user;
		this->pass = other.pass;
		this->mail = other.mail;
		this->limits = other.limits;
	}

	std::string user;
	std::string pass;
	std::string mail;
	std:: map<std::string, int> limits;
};

class XMLParser
{
public:
	XMLParser(std::string filePath);

	/*
	To fetch the data of a particular refrenced by 
	@param key unique key for the client.
	@return Data of client in a ClientData instance.
	*/
	ClientData getClientData(std::string key);

	/*
	Add data of a client in data store.
	@param key unique key for a client
	@param data Data of client in a ClientData instance.
	*/
	int addClientData(std::string key, ClientData data);

private:
	/*
	Parse the xml database.
	Store the data of all clients in a map @param m_clientData .
	*/
	void populateData();

	/*
	map uniqueKey => ClientData
	*/
	std::map<std::string, ClientData> m_clientData;

	// XML Parser class
	CMarkup xml;
};
