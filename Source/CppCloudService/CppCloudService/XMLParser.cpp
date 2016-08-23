#include "XMLParser.h"

//To be removed later
#include <iostream>

XMLParser::XMLParser(std::string filePath)
{
	xml.Load(filePath);
	populateData();
	std::cout << "\nSize of map is: " << m_clientData.size() << std::endl;
}

void XMLParser::populateData()
{
	while (xml.FindElem("client"))
	{
		std::string user = xml.GetAttrib("login");
		std::string pass = xml.GetAttrib("pass");
		std::string mail = xml.GetAttrib("mail");
		std::cout << "\nuser: " << user << "\tpass: " << pass << "\tmail: " << mail << "\t";
		int limit = -1;
		std::map<std::string, int> limits;
		xml.IntoElem();
		while (xml.FindElem("alert"))
		{
			std::string type = xml.GetAttrib("type");
			std::string value = xml.GetAttrib("limit");
			if (type == "memory" || type == "cpu") {
				value.pop_back();		// Remove '%' at the end of the value
			}

			limit = atoi(value.c_str());
			std::cout << "\t" << type << " limit: " << limit;
			limits[type] = limit;
		}
		ClientData d(user, pass, mail, limits);
		std::string key = user + pass;
		addClientData(key, d);
		xml.OutOfElem();
	}

}

int XMLParser::addClientData(std::string k, ClientData d)
{
	if (m_clientData.find(k) != m_clientData.end())
	{
		std::cout << "Element with key=" << k << " already exists in list. So, skipping it." << std::endl;
		return -1;
	}
	else {
		m_clientData[k] = d;
		return 1;
	}
}

ClientData XMLParser::getClientData(std::string k)
{
	return m_clientData[k];
}