# System Monitor

It's a network system monitoring application. It can monitor the systems on a network,
You can run 'client' program on a system. It collect the system usage data and sends it to a service ( A http sever ) and then exits.
client identifies itself to service using a 'username and password'.
You can confiure all clients' username and password and their limits for system usage on server ( where the service is run ) in an xml file.
The service compares the data sent by client to the limits that we have configured for that client on server. And if the client
fails to comply with the limits of system usage, an email notification is sent to the client alerting him about the issue.

Run the service on a server and it will process the data sent by clients.
You can schedule to run the 'client' program ( like at an interval of 2 minutes ) using a task scheduler.
It will keep service informed about system usage. This way you can monitor systems on a network.

Note: Please refer to 'config' file details below to setup the application.

There are two seperate VC++ projects in 'source' directory :
1.      Client ( To be run on client machines. Collects data from system and sends it to service to process )
2.      CPPCloudService ( A HTTP server, using Casablanca - C++ REST SDK, to monitor systems )

========================================
To compile it in Visual Studio 2015
========================================

1. Please add C++ REST SDK ( Casablanca ) to the projects using NuGet package manager.
	Package name is: "cpprestsdk.v140.windesktop.msvcstl.dyn.rt-dyn.2.8.0"
	Find details at https://github.com/Microsoft/cpprestsdk/wiki/How-to-use-the-C---Rest-SDK-NuGet-package .
2. If the standard include files ( like iostream, stdio.h etc. ) are not included in the project. Then follow below steps:
   ( Reason being https://blogs.msdn.microsoft.com/vcblog/2015/03/03/introducing-the-universal-crt/ ) .
	1.1  Go to Project->Properties->. 
	1.2  In Project Properties->VC++ Diretories->Library Directories.
		 Add a path to C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10150.0\ucrt\
		 Note: In the path for libs Choose your architecture ( x86/x64 ).
	1.3 And in C/C++->General->Additional include directories add a path to
		C:\Program Files (x86)\Windows Kits\10\Include\10.0.10150.0\ucrt .
		Note: In the above path "10.0.10150.0" may vary depending on your version. ( "10.0.10150.0" is for my system )
		
NOTE: As I have tested in Visual Studio 2013, it doesn't add the C++ REST SDK propery.
	If you have to work on Visual Studio 2013. Then please copy all the files for "C++ REST SDK" package in 
	"packages" directory of your project. Then add the "include" and "library" path manually in your
	project configuration similar to what has been described in Point 2 above.
	
========================================
Config files.
========================================
1. "config.txt" for client. The config file contains a keyword and then their value separated by space ( or tab ). 
	Sample contents of the file are given below:
		user	user2
		pass	aaa2
		address	http://localhost:11426
		
	Username and password of a client using which client identifies itself to server. 'address' is the address of http server.
	Note: Copy this file to directory from where executable (client.exe) for the client program is executed.
	
2. "config.txt" for CPPCloudService. It contains email, password ( for the email ), address which is used to host http server.
	Sample contents of this are:
		email	email@example.com
		pass	examplePass
		address	http://localhost:11426
	A gmail address and it's password. This details are used to send emails to clients.
	Allow less secure apps to be able to send emails. Please refer to https://support.google.com/accounts/answer/6010255?hl=en .
3. "clients.xml" for data of clients ( login, password, email etc);
	Sample contents of this file are:
		<?xml version="1.0"?>
		<client login="user1" pass="aaa1", mail="abhgang@gmail.com">
			<alert type="memory" limit="50%" />
			<alert type="cpu" limit="60%" />
			<alert type="processes" limit="30" />
		</client>
		<client login="user2" pass="aaa2", mail="abhnith7@gmail.com">
			<alert type="memory" limit="2%" />
			<alert type="cpu" limit="97%" />
			<alert type="processes" limit="300" />
		</client>
Note: These configuration files should be placed in same directory as the directory from where 
	the executable (CPPCloudService.exe/Client.exe) of program is executed.
	If you’re compiling directly from Visual studio, copy these file in project directory where all .cpp and .h files are located.
