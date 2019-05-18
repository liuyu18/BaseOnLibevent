
#pragma once
#include "XFtpTask.h"
#include <map>
class XFtpServerCMD :public XFtpTask
{
public:
	virtual bool Init();
	virtual void Read(struct bufferevent *bev);
	virtual void Event(struct bufferevent *bev, short what);
	void Reg(std::string, XFtpTask *call);

	XFtpServerCMD();
	~XFtpServerCMD();
private:
	std::map<std::string, XFtpTask*>calls;
	std::map< XFtpTask*, int>calls_del;
};

