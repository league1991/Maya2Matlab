#include "serverstdafx.h"

int nInput = 1;
class ReadObjTask:public ExecTask
{
public:
	ReadObjTask()
	{
		for (int ithIn = 0; ithIn < nInput; ++ithIn)
		{
			addInputSlot(QString("in%1").arg(ithIn));
		}
		addOutputSlot("out0");
	}
	QString name(){return "testTask";}
	bool compute()
	{
		MatrixData* in0 = getInputData("in0");
		MatrixData* out0 = getOutputData("out0");
		(*out0) = (*in0);

// 		double* p,*p1;
// 		int r,c;
// 		MatrixData::DataType t;
// 		out0->getDenseDataPtr((void*&)p, r,c,t);
// 		for (int ithIn = 0; ithIn < nInput; ++ithIn)
// 		{
// 			MatrixData* in1 = getInputData(QString("in%1").arg(ithIn));
// 			in1->getDenseDataPtr((void*&)p1, r,c,t);
// 			p[0] = p[0] + p1[1];
// 		}
// 		
// 		qDebug() << p[0] << p[2];
		m_infoBuffer = "computation finished!\n";
		return true;
	}
};

void main(int argc, char*argv[])
{
// 	Engine* eng = engOpen(0);
// 
// 	mxArray*   mArray;
// 	for (int i = 0; i < 100000; ++i)
// 	{
// // 		MatrixData m;
// // 		m.getFromEngine(eng, "b", mArray);
// // 		mxDestroyArray(mArray);
// // 		m.sendToEngine(eng,"c", mArray);
// // 		mxDestroyArray(mArray);
// 
// 		StructData s;
//  		s.getFromEngine(eng, "s");
// 
// 		QByteArray bytes;
// 		QDataStream dr(&bytes, QIODevice::ReadWrite);
// 		dr << s;
// 
// 		QDataStream dw(&bytes, QIODevice::ReadWrite);
// 		StructData s2;
// 		dw >> s2;
// 
// 
// 		StructData s3;
// 		s3 = s2;
// 		s3.sendToEngine(eng, "f1");
// 	}
// 	//mxDestroyArray(mArray);
// 	system("pause");
	QApplication app(argc, argv);
	ExeServer server;
	ExecTask::Ptr pTask = ExecTask::Ptr(new ReadObjTask);
	server.addTask(pTask);
	server.setPort(12111);
	server.start();
	app.exec();
}