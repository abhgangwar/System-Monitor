#include "SmtpLib.h"

SmtpLib::SmtpLib(const char* userName, const char* password) :
	m_emailAddress(userName),
	m_password(password)
{
	::CoInitialize(NULL);
	m_smtpClient.CreateInstance("EASendMailObj.Mail");
	m_smtpClient->LicenseCode = _T("TryIt");

	// Using Gmail SMTP Server
	// Gmail SMTP server address
	m_smtpClient->ServerAddr = _T("smtp.gmail.com");
	m_smtpClient->ServerPort = 587;

	// detect SSL/TLS automatically
	m_smtpClient->SSL_init();

	// Gmail user authentication
	m_smtpClient->UserName = _T(m_emailAddress);
	m_smtpClient->Password = _T(m_password);
}

int SmtpLib::sendEmail(const char* from, const char* to, const char* subject, const char* body)
{
	m_smtpClient->FromAddr = _T(from);

	// Add recipient email address
	m_smtpClient->AddRecipientEx(_T(to), 0);

	// Set email subject
	m_smtpClient->Subject = _T(subject);

	// Set email body
	m_smtpClient->BodyText = _T(body);

	if (m_smtpClient->SendMail() == 0)
	{
		printf("%s",_T("email was sent successfully!\r\n"));
		return 1;
	}
	else
	{
		printf(_T("failed to send email with the following error: %s\r\n"),
			(const TCHAR*)m_smtpClient->GetLastErrDescription());
		return 0;
	}
}