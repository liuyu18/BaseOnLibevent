#define _CRT_SECURE_NO_WARNINGS

#include "XFtpLIST.h"
#include <iostream>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <string>
#include <io.h>

using namespace std;

void XFtpLIST::Write(struct bufferevent *bev)
{

	ResCMD("226 Transfer complete\r\n");
	Close();

}
void XFtpLIST::Event(struct bufferevent *bev, short what)
{

	if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
	{
		cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT" << endl;
		Close();
	}
	else if (what&BEV_EVENT_CONNECTED)
	{
		cout << "XFtpLIST BEV_EVENT_CONNECTED" << endl;
	}
}

void XFtpLIST::Parse(std::string type, std::string msg)
{
	string resmsg = "";

	if (type == "PWD")
	{
		resmsg = "257 \"";
		resmsg += cmdTask->curDir;
		resmsg += "\" is current dir.\r\n";

		ResCMD(resmsg);
	}
	else if (type == "LIST")
	{
		ConnectPORT();
		ResCMD("150 Here comes the directory listing.\r\n");
		string listdata = GetListData(cmdTask->rootDir + cmdTask->curDir);
		Send(listdata);
	}
	else if (type == "CWD") 
	{
		int pos = msg.rfind(" ") + 1;
		string path = msg.substr(pos, msg.size() - pos - 2);
		if (path[0] == '/') 
		{
			cmdTask->curDir = path;
		}
		else
		{
			if (cmdTask->curDir[cmdTask->curDir.size() - 1] != '/')
				cmdTask->curDir += "/";
			cmdTask->curDir += path + "/";
		}

		ResCMD("250 Directory succes chanaged.\r\n");

	}
	else if (type == "CDUP") 
	{

		string path = cmdTask->curDir;

		if (path[path.size() - 1] == '/')
		{
			path = path.substr(0, path.size() - 1);
		}
		int pos = path.rfind("/");
		path = path.substr(0, pos);
		cmdTask->curDir = path;
		ResCMD("250 Directory succes chanaged.\r\n");
	}
}
std::string XFtpLIST::GetListData(std::string path)
{

	string data = "";
	_finddata_t file;

	path += "/*.*";
	intptr_t dir = _findfirst(path.c_str(), &file);
	if (dir < 0)
		return data;
	do
	{
		string tmp = "";
		if (file.attrib &_A_SUBDIR)
		{
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
			{
				continue;
			}
			tmp = "drwxrwxrwx 1 root group ";
		}
		else
		{
			tmp = "-rwxrwxrwx 1 root group ";
		}

		char buf[1024];
		sprintf_s(buf, "%u ", file.size);
		tmp += buf;
		strftime(buf, sizeof(buf) - 1, "%b %d %H:%M ", localtime(&file.time_write));
		tmp += buf;
		tmp += file.name;
		tmp += "\r\n";
		data += tmp;

	} while (_findnext(dir, &file) == 0);



	return data;
}