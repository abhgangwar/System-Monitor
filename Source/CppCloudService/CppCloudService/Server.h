#pragma once
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

#include "SmtpLib.h"
#include "XMLParser.h"

using namespace utility;									// Common utilities like string conversions
using namespace web;										// Common features like URIs.
using namespace web::http;									// Common HTTP functionality
using namespace web::http::experimental::listener;          // HTTP server
using namespace web::json;                                  // JSON library


class Server : public http_listener
{
public:
	Server(string_t url, const char* email, const char* pass);

	/*
	Handler for post requests of this server.
	@param req http request object
	*/
	void handlePostReq(http_request req);
	/*
	Fetch the data of a particular client.
	@param key unique key for a client.
	@return data of clinet in a ClientData class instance
	*/
	ClientData getClientData(std::string key);

	/*
	To start the server.
	*/
	pplx::task<void> open();

	/*
	To close the server.
	*/
	pplx::task<void> close();

private:
	static void handle_error(pplx::task<void>& t);			// Error Handling
	http_listener m_listener;
	SmtpLib m_smtpClient;
	XMLParser m_xml;
};
