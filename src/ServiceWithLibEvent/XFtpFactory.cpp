
#include "XFtpFactory.h"
#include "XFtpServerCMD.h"
#include "XFtpUSER.h"
#include "XFtpLIST.h"
#include "XFtpPORT.h"
XTask *XFtpFactory::CreateTask()
{
	XFtpServerCMD *x = new XFtpServerCMD();
	x->Reg("USER", new XFtpUSER());
	XFtpLIST * list = new XFtpLIST();
	x->Reg("PWD", list);
	x->Reg("PORT", new XFtpPORT());

	return x;
}

XFtpFactory::XFtpFactory()
{
}


