// ExeClient.cpp : 定义控制台应用程序的入口点。
//

#include "serverstdafx.h"
#include "clientstdafx.h"

#define DATA_PACKET_HEADER    "MatlabPlugin Node Data Header"

int nInput = 1000;
int main(int argc, char* argv[])
{
	MatrixData mat;
	int rSize = 150;
	float *a = new float[rSize * rSize];
	for (int i = 0; i < rSize*rSize; ++i)
	{
		a[i]=1;
	}
	mat.setDenseData(a,rSize,rSize,MatrixData::DT_FLOAT);
	QHash<QString, MatrixData*> input;
	input["in0"] = &mat;

	for (int ithIn = 0; ithIn < nInput; ithIn++)
	{
		MatrixData* dat = new MatrixData;
		(*dat) = mat;
		input[QString("in%1").arg(ithIn)] = dat;		
	}

	for (int i = 0; i < 100000; ++i)
	{
		ExeClient client;
		if(!client.connectToServer("125.216.240.121",12111))//"125.216.241.184"    "192.168.1.118"
			return 0;

		QHash<QString, QSharedPointer<MatrixData>> output;
		output.clear();
		if (!client.taskBegin("testTask", nInput, 10, 5))
		{
			qDebug("failed to begin");goto END;
		}
		else if (!client.sendInput(input))
		{
			qDebug("failed to send input");goto END;
		}
		else if (!client.compute())
		{
			qDebug("failed to compute");goto END;
		}
		else if (!client.receiveOutput(output))
		{
			qDebug("failed to receive output");goto END;
		}
		else if (!client.taskEnd())
		{
			qDebug("failed to end task");goto END;
		}

		*(input["in0"]) = *(output["out0"]);
		float *p;
		int nR, nC;
		MatrixData::DataType type;
		input["in0"]->getDenseDataPtr((void*&)p, nR, nC, type);
		qDebug("%f %f %f %f", p[0],p[1],p[2],p[3]);
END:
		client.disconnectFromServer();
	}

	system("pause");
	return 0;
}

