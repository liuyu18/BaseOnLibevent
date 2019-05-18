#include <event2/event.h>
#include <event2/listener.h>
#include <string.h>
#include "XThreadPool.h"
#ifndef _WIN32
#include <signal.h>
#endif
#include <iostream>
#include "XFtpFactory.h"
using namespace std;
#define SPORT 21

void listen_cb(struct evconnlistener * e, evutil_socket_t s, struct sockaddr *a, int socklen, void *arg) {
	cout << "listen_cb" << endl;
	XTask *task = XFtpFactory::Get()->CreateTask();
	task->sock = s;
	XThreadPool::Get()->Dispatch(task);
}

int main() {
#ifdef _WIN32 
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return 1;
#endif

	XThreadPool::Get()->Init(10);
	std::cout << "test thread pool!\n";
	event_base *base = event_base_new();
	if (base) {
		cout << "event_base_new success!" << endl;
	}
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SPORT);
	evconnlistener *ev = evconnlistener_new_bind(base,
		listen_cb, base, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
		10, (sockaddr*)&sin,
		sizeof(sin)
	);
	if (base)
		event_base_dispatch(base);
	if (ev)
		evconnlistener_free(ev);
	if (base)
		event_base_free(base);
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;



}