#include "stdafx.h"
#include "clientStdAfx.h"
#include "serverstdafx.h"
#include "ExeClient.h"
#include <exception>

ExeClient::ExeClient(void)
{
}

ExeClient::~ExeClient(void)
{
}

bool ExeClient::connectToServer(const QString& serverName, const quint16& serverPort)
{
	if (m_socket.state() != QAbstractSocket::UnconnectedState)
	{
		disconnectFromServer();
	}
	m_serverName = serverName;
	m_serverPort = serverPort;
	m_socket.connectToHost(m_serverName, m_serverPort);
	if(!m_socket.waitForConnected(CONNECT_SERVER_WAITTIME))
	{
		m_infoStr += "failed to connect to server\n";
		return false;
	}
	m_infoStr += "connected\n";
	return true;
}

bool ExeClient::waitForDataSent( const QByteArray& data , const int maxSeconds)
{
	if (m_socket.state() != QAbstractSocket::ConnectedState || !m_socket.isValid())
		return false;

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(DATA_STREAM_VERSION);

	out << (quint32)0;
	out << data;
	out.device()->seek(0);
	quint32 dataSize = block.size() - sizeof(quint32);
	out << dataSize;
	
	m_socket.write(block);
	if (!m_socket.waitForBytesWritten())
		return false;
	return true;
}

bool ExeClient::waitForDataRead( QByteArray& data, const int maxSeconds /*= 10*/ )
{
	if (m_socket.state() != QAbstractSocket::ConnectedState || !m_socket.isValid())
		return false;

	while (m_socket.bytesAvailable() < (int)sizeof(quint32))
	{
		if (!m_socket.waitForReadyRead(maxSeconds * 1000)) 
			return false;
	}

	quint32 blockSize;
	QDataStream in(&m_socket);
	in.setVersion(DATA_STREAM_VERSION);
	in >> blockSize;

	qint64 bytesAvai = m_socket.bytesAvailable();
	while (bytesAvai < blockSize) {
		if (!m_socket.waitForReadyRead(maxSeconds * 1000)) 
			return false;
		bytesAvai = m_socket.bytesAvailable();
	}

	if (in.status() == QDataStream::Ok)
	{
		data.clear();
		// to garantee enough space to allocate
		char *buf = NULL;
		try
		{
			buf = new char[int(bytesAvai * 1.5 + 10)];
		}
		catch (const std::bad_alloc &ex)
		{
			return false;
		}
		if (buf)
			delete[] buf;
		in >> data;
		return true;
	}
	return false;
}

bool ExeClient::disconnectFromServer()
{
	m_socket.disconnectFromHost();
	if (m_socket.state() == QAbstractSocket::UnconnectedState ||
		m_socket.waitForDisconnected(CONNECT_SERVER_WAITTIME))
		return true;
	return false;
}

bool ExeClient::isConnectionValid()
{
	return m_socket.state() == QAbstractSocket::ConnectedState && 
		   m_socket.isValid();
}

QList<QHostAddress> ExeClient::getIPList()
{
	return QNetworkInterface::allAddresses();
}

bool ExeClient::taskBegin( const QString& taskName, unsigned nInputs, unsigned maxTransferSeconds, unsigned maxComputeSeconds )
{
	TaskBeginPacket reqPacket(taskName, nInputs, maxTransferSeconds, maxComputeSeconds);
	ReplyPacket     replyPacket;
	QByteArray      reqByte, replyByte;
	bool            isValid = true;
	m_maxTransferTime = maxTransferSeconds;

	if(!reqPacket.writeToBuffer(reqByte))
	{
		isValid = false;
		m_infoStr += "failed to write packet\n";
	}
	else if (!waitForDataSent(reqByte, 10))
	{
		isValid = false;
		m_infoStr += "failed to send data\n";
	}
	else if (!waitForDataRead(replyByte, 10) ||
		     !replyPacket.readFromBuffer(replyByte)    ||
		     !replyPacket.isValid())
	{
		isValid = false;
		m_infoStr += "no reply\n";
	}
	else
		m_infoStr += "task begin\n";

	qDebug() << m_infoStr;
	m_curTask = reqPacket;
	return isValid;
}

bool ExeClient::sendInput( QHash<QString, MatrixData*> inputArray )
{
	if (inputArray.size() != m_curTask.nInputs())
	{
		m_infoStr += "number of inputs doesn't match.\n";
		return false;
	}

	QHash<QString, MatrixData*>::iterator pIn;
	for (pIn = inputArray.begin(); pIn != inputArray.end(); ++pIn)
	{
		QString inputName = pIn.key();
		MatrixData* data  = pIn.value();
		if (!data)
		{
			m_infoStr += "null data\n";
			return false;
		}
		VariablePacket varPacket(inputName, data);
		ReplyPacket    replyPacket;
		QByteArray byte, replyByte;
		if (!varPacket.writeToBuffer(byte))
		{
			m_infoStr += "failed to write to buffer\n";
			return false;
		}
		if (!waitForDataSent(byte, m_curTask.maxTransferSeconds()))
		{
			m_infoStr += "failed to transfer input\n";
			return false;
		}
		if (!waitForDataRead(replyByte, m_curTask.maxTransferSeconds()) ||
			!replyPacket.readFromBuffer(replyByte) ||
			!replyPacket.isValid())
		{
			m_infoStr += "no reply / failed to read from buffer / not a valid variable\n";
			return false;
		}
	}
	m_infoStr += "input sent\n";
	return true;
}

bool ExeClient::sendInput( const QString& varName, MatrixData* matData )
{
	const QString& inputName = varName;
	MatrixData* data  = matData;
	if (!data)
	{
		m_infoStr += "null data\n";
		return false;
	}
	VariablePacket varPacket(inputName, data);
	ReplyPacket    replyPacket;
	QByteArray byte, replyByte;
	if (!varPacket.writeToBuffer(byte))
	{
		m_infoStr += "failed to write to buffer\n";
		return false;
	}
	if (!waitForDataSent(byte, m_curTask.maxTransferSeconds()))
	{
		m_infoStr += "failed to transfer input\n";
		return false;
	}
	if (!waitForDataRead(replyByte, m_curTask.maxTransferSeconds()) ||
		!replyPacket.readFromBuffer(replyByte) ||
		!replyPacket.isValid())
	{ 
		m_infoStr += "no reply / failed to read from buffer / not a valid variable\n";
		return false; 
	}
	m_infoStr += "input " + varName + " sent\n";
	return true;
}

bool ExeClient::compute()
{
	BeginComputePacket begCmpPacket;
	EndComputePacket   endCmpPacket;
	QByteArray         cmpBuffer, replyBuffer;
	if (!begCmpPacket.writeToBuffer(cmpBuffer))
	{
		m_infoStr += "faild to write to buffer\n";
		return false;
	}
	if (!waitForDataSent(cmpBuffer, m_curTask.maxTransferSeconds()))
	{
		m_infoStr += "failed to request compute\n";
		return false;
	}
	if (!waitForDataRead(replyBuffer, m_curTask.maxComputeSeconds()) ||
		!endCmpPacket.readFromBuffer(replyBuffer))
	{
		m_infoStr += "failed to get reply\n";
		return false;
	}

	m_infoStr += endCmpPacket.info();
	m_curCmpRes = endCmpPacket;
	return m_curCmpRes.isSuccess();
}

bool ExeClient::receiveOutput( QHash<QString, QSharedPointer<MatrixData>>& outputArray )
{
	int nOutputs = m_curCmpRes.nOutputs();
	bool isValid = true;
	for (int ithOut = 0; ithOut < nOutputs; ++ithOut)
	{
		QSharedPointer<MatrixData> pDat = QSharedPointer<MatrixData>(new MatrixData);
		VariablePacket packet("", pDat.data());

		QByteArray outBuf, replyBuf;
		if (!waitForDataRead(outBuf, m_curTask.maxTransferSeconds()))
		{
			m_infoStr += "failed to receive data\n";
			isValid = false;
		}
		else if (!packet.readFromBuffer(outBuf) ||
			!packet.isValid())
		{
			m_infoStr += "invalid output\n";
			isValid = false;
		}
		else
			outputArray[packet.variableName()] = pDat;

		ReplyPacket    replyPacket(isValid, m_infoStr);
		replyPacket.writeToBuffer(replyBuf);
		isValid &= waitForDataSent(replyBuf, m_curTask.maxTransferSeconds());
		if (!isValid)
			return false;
	}
	m_infoStr += "output sent\n";
	return true;
}

bool ExeClient::receiveOutput( QString& name, MatrixData* pDat )
{
	VariablePacket packet("", pDat);

	bool isValid = true;
	QByteArray outBuf, replyBuf;
	if (!waitForDataRead(outBuf, m_curTask.maxTransferSeconds()))
	{
		m_infoStr += "failed to receive data\n";
		isValid = false;
	}
	else if (!packet.readFromBuffer(outBuf) || !packet.isValid())
	{
		m_infoStr += "invalid output\n";
		isValid = false;
	}
	else
		name = packet.variableName();

	ReplyPacket    replyPacket(isValid, m_infoStr);
	replyPacket.writeToBuffer(replyBuf);
	isValid &= waitForDataSent(replyBuf, m_curTask.maxTransferSeconds());
	if (isValid)
		m_infoStr += "output " + name + " received\n";
	return isValid;
}

bool ExeClient::taskEnd()
{
	TaskEndPacket packet;
	QByteArray    endByte;
	if (!waitForDataRead(endByte, m_curTask.maxTransferSeconds()) ||
		!packet.readFromBuffer(endByte))
	{
		m_infoStr += "failed to end task\n";
		return false;
	}

	m_infoStr += "task ended\n";
	return true;
}
