// ObjReader.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	ExeServer server;
	ExecTask::Ptr pTask = ExecTask::Ptr(new ReadObjTask("readObj"));
	ExecTask::Ptr pPlyTask = ExecTask::Ptr(new WritePlyTask("writePly"));
	server.addTask(pTask);
	server.addTask(pPlyTask);
	server.setPort(12345);
	server.start();
	app.exec();
	return 0;
}

