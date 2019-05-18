
#pragma once
#include "XTask.h"
#include <string>
class XFtpTask :public XTask
{
public:
	std::string curDir = "/";
	std::string rootDir = ".";
	std::string ip = "";
	int port = 0;
	XFtpTask *cmdTask = 0;
	virtual void Parse(std::string type, std::string msg) {}
	void ResCMD(std::string msg);
	void Send(std::string data);
	void Send(const char*data, int datasize);
	void ConnectPORT();
	void Close();
	virtual void Read(struct bufferevent *bev) {}
	virtual void Write(struct bufferevent *bev) {}
	virtual void Event(struct bufferevent *bev, short what) {}
	void SetCallback(struct bufferevent *bev);
	bool Init() { return true; }
protected:
	static void ReadCB(bufferevent * bev, void *arg);
	static void WriteCB(bufferevent * bev, void *arg);
	static void EventCB(struct bufferevent *bev, short what, void *arg);
	struct bufferevent *bev = 0;
	FILE *fp = 0;


};

