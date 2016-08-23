#pragma once
#ifndef SMTP_LIB

#include "easendmailobj.tlh"
#include <tchar.h>

using namespace EASendMailObjLib;

class SmtpLib
{
public:
	SmtpLib(const char* userName, const char* password);

	/*
	Send email with given parameters.
	@param from From  address for email.
	@param to Receipent
	@subject Subject of email
	@body Body of email
	*/
	int sendEmail(const char* from, const char* to, const char* subject, const char* body);

private:
	IMailPtr m_smtpClient;

	// Email credentials
	const char* m_emailAddress;
	const char* m_password;
};

#endif // !SMTP_LIB