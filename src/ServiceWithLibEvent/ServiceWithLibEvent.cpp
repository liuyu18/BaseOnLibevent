// ServiceWithLibEvent.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <event2/event.h>
#include <event2/thread.h>
#include<event2/listener.h>
#include <string.h>
#ifndef _WIN32
#include <signal.h>
#endif
#include <iostream>
using namespace std;
#define SPORT 5001

using namespace std;



void listen_cb(struct evconnlistener * e, evutil_socket_t s, struct sockaddr *a, int socklen, void *arg) {
	cout << "listen_cb" << endl;
}

int main()
{
#ifdef _WIN32 
	//初始化socket库
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

	event_config *conf = event_config_new();

	const char **methods = event_get_supported_methods();
	cout << "支持的 方法" << endl;
	for (int i = 0; methods[i] != nullptr; i++)
	{
		cout << methods[i] << endl;
	}

#ifdef _WIN32
	event_config_set_flag(conf, EVENT_BASE_FLAG_STARTUP_IOCP);
	evthread_use_windows_threads();

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	event_config_set_num_cpus_hint(conf, si.dwNumberOfProcessors);
#endif

	event_config_require_features(conf, EV_FEATURE_FDS);

	event_base *base = event_base_new_with_config(conf);
	event_config_free(conf);

	if (!base) {
		cerr << "event_base_new_with_config failed!" << endl;
		base = event_base_new();
		if (!base)
		{
			cerr << "event_base_new failed！" << endl;
			return 0;
		}
	}
	else {
		int f = event_base_get_features(base);
		if (f&EV_FEATURE_ET)
			cout << "EV_FEATURE_ET events are supported." << endl;
		else
			cout << "EV_FEATURE_ET events are not supported." << endl;
		if (f&EV_FEATURE_O1)
			cout << "EV_FEATURE_O1 events are supported." << endl;
		else
			cout << "EV_FEATURE_O1 events are not supported." << endl;
		if (f&EV_FEATURE_FDS)
			cout << "EV_FEATURE_FDS events are supported." << endl;
		else
			cout << "EV_FEATURE_FDS events are not supported." << endl;

		if (f&EV_FEATURE_EARLY_CLOSE)
			cout << "EV_FEATURE_EARLY_CLOSE events are supported." << endl;
		else
			cout << "EV_FEATURE_EARLY_CLOSE events are not supported." << endl;
		cout << "event_base_new_with_config success!" << endl;



		sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(SPORT);

		evconnlistener * ev = evconnlistener_new_bind(base, listen_cb, base, 10,
			LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
			(sockaddr*)&sin, sizeof(sin));
		event_base_dispatch(base);
		evconnlistener_free(ev);

		event_base_free(base);
	}

#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
