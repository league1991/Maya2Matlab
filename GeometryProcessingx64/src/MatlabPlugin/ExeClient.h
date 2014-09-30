#pragma once

#define CONNECT_SERVER_WAITTIME 3000

class ExeClient
{
public:
	ExeClient(void);
	virtual ~ExeClient(void);

	QList<QHostAddress> getIPList();

	bool isConnectionValid();
	bool connectToServer(const QString& serverName, const quint16& serverPort);
	bool disconnectFromServer();

	QString getSocketErrorString(){return m_socket.errorString();}
	QString getInfo(){return m_infoStr;}
	void    clearInfo(){m_infoStr = "";}

	bool taskBegin(const QString& taskName, unsigned nInputs, unsigned maxTransferSeconds, unsigned maxComputeSeconds);
	bool sendInput(QHash<QString, MatrixData*> inputArray);
	bool sendInput(const QString& varName, MatrixData* matData);
	bool compute();
	bool receiveOutput(QHash<QString, QSharedPointer<MatrixData>>& outputArray);
	bool receiveOutput( QString& name, MatrixData* pDat );
	bool taskEnd();

private:
	bool waitForDataSent(const QByteArray& data, const int maxSeconds = 10);
	bool waitForDataRead(QByteArray& data, const int maxSeconds = 10);

	QTcpSocket m_socket;
	QString    m_serverName;
	quint16    m_serverPort;

	unsigned   m_maxTransferTime;
	QString    m_infoStr;
	TaskBeginPacket m_curTask;
	EndComputePacket m_curCmpRes;
};
