#include "StdAfx.h"
#include "clientstdafx.h"
#include "serverstdafx.h"
#include "ExeServer.h"

ExeServer::ExeServer(void)
{
	m_maxSecondsToWait = 120;
	m_server.moveToThread(this);
	m_port = 0;
}

ExeServer::~ExeServer(void)
{
}

void ExeServer::run()
{
	if(!m_server.listen(QHostAddress::Any, m_port))
		return;

	printServerInfo();
	while (1)
	{
		if (!m_server.waitForNewConnection(-1))
			break;

		if (!m_server.hasPendingConnections())
			continue;
		QTcpSocket* socket = m_server.nextPendingConnection();
		qDebug("new connection!");
// 
// 		QString     errorString; 
// #define CHECK_AND_GOTO(x,str) if(x){errorString = str;goto ERROR_HANDLE;}
// 
// 		QByteArray  inData;
// 		QByteArray  outData;
// 		CHECK_AND_GOTO(!waitForDataRead(*socket, inData, m_maxSecondsToWait), "no data");
// 
// 		QDataStream inStream(inData);
// 		QDataStream outStream(&outData, QIODevice::WriteOnly);
// 		inStream.setVersion(DATA_STREAM_VERSION);
// 		outStream.setVersion(DATA_STREAM_VERSION);
// 
// 
// 		QString     header;
// 		QString     taskName;
// 		ExecTask::Ptr task;
// 
// 		inStream >> header;
// 		CHECK_AND_GOTO(header != DATA_PACKET_HEADER, "header invalid.")
// 
// 		inStream >> taskName;
// 		CHECK_AND_GOTO(!m_taskList.contains(taskName),"no such task")
// 
// 		task = m_taskList[taskName];
// 		CHECK_AND_GOTO(task.isNull(), "null task ptr")
// 		CHECK_AND_GOTO(!(task->readInputFromStream(inStream)),"failed to read input data")		
// 		CHECK_AND_GOTO(!task->compute(), "computation error")
// 
// 		outStream << QString(DATA_PACKET_HEADER) << taskName;
// 		CHECK_AND_GOTO(!task->writeOutputToStream(outStream), "failed to write output data")
// 
// 		CHECK_AND_GOTO(!waitForDataSent(*socket, outData, m_maxSecondsToWait), "failed to send output")
// 		qDebug("task finished!\n");
// 		goto CLEAR_SOCKET;
// 
// ERROR_HANDLE:
// 		sendErrorReport(*socket, errorString);
// CLEAR_SOCKET:
// 		socket->disconnectFromHost();
// 		if (socket->state() == QAbstractSocket::UnconnectedState ||	socket->waitForDisconnected(1000))
// 			qDebug("Disconnected!");
// 		inData.clear();
// 		outData.clear();

		if (!socket                ||
			!taskBegin(*socket)    ||
			!receiveInput(*socket) ||
			!compute(*socket)      ||
			!sendOutput(*socket)   ||
			!taskEnd(*socket))
			qDebug("error occurs");

		socket->disconnectFromHost();
		if (socket->state() == QAbstractSocket::UnconnectedState ||	socket->waitForDisconnected(1000))
			qDebug("Disconnected!");

		delete socket;
	}
}

void ExeServer::sendErrorReport( QTcpSocket& socket, const QString & errorString )
{
	QByteArray errorByte;
	QDataStream errorStream;
	errorStream << QString(ERROR_PACKET_HEADER) << errorString;
	waitForDataSent(socket, errorByte, m_maxSecondsToWait);
}

void ExeServer::addTask( ExecTask::Ptr task )
{
	m_taskList[task->name()] = task;
}

bool ExeServer::waitForDataRead(QTcpSocket& socket, QByteArray& data, const int maxSeconds /*= 10*/ )
{
	if (socket.state() != QAbstractSocket::ConnectedState || !socket.isValid())
		return false;

	while (socket.bytesAvailable() < (int)sizeof(quint32))
	{
		if (!socket.waitForReadyRead(maxSeconds * 1000)) 
			return false;
	}

	quint32 blockSize;
	QDataStream in(&socket);
	in.setVersion(DATA_STREAM_VERSION);
	in >> blockSize;

	int bytesAvai = socket.bytesAvailable();
	while (socket.bytesAvailable() < blockSize) {
		if (!socket.waitForReadyRead(maxSeconds * 1000)) 
			return false;
	}

	data.clear();
	in >> data;
	return true;
}

bool ExeServer::waitForDataSent(QTcpSocket& socket, const QByteArray& data , const int maxSeconds)
{
	if (socket.state() != QAbstractSocket::ConnectedState || !socket.isValid())
		return false;

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(DATA_STREAM_VERSION);

	out << (quint32)0;
	out << data;
	out.device()->seek(0);
	out << (quint32)(block.size() - sizeof(quint32));

	socket.write(block);
	return socket.waitForBytesWritten(maxSeconds*1000);
}

void ExeServer::setMaxSecondsToWait( int maxSecondsToWait )
{
	m_maxSecondsToWait = maxSecondsToWait;
}

void ExeServer::printServerInfo()
{
 	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
		// use the first non-localhost IPv4 address
	for (int i = 0; i < ipAddressesList.size(); ++i) {
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address()) {
				ipAddress = ipAddressesList.at(i).toString();
				break;
		}
	}
	// if we did not find one, use IPv4 localhost
	if (ipAddress.isEmpty())
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
	qDebug() << QString("The server is running.\n"
		                "IP: %1 \n"
						"port: %2\n"
						"max seconds to wait: %3\n"
		               ).arg(ipAddress).arg(m_server.serverPort()).arg(m_maxSecondsToWait);


	for (QHash<QString, ExecTask::Ptr>::ConstIterator pTask = m_taskList.constBegin();
		 pTask != m_taskList.constEnd(); ++pTask)
	{
		QString str = pTask.value()->getTaskInfo();
		qDebug() << str;
	}
}

void ExeServer::setPort( quint16 port )
{
	m_port = port;
}

bool ExeServer::taskBegin( QTcpSocket& socket)
{
	QByteArray buffer;
	bool isValid = true;
	QString infoStr;
	if(!waitForDataRead(socket, buffer, m_maxSecondsToWait))
	{
		isValid = false;
		infoStr = "data read error";
	}
	else if(!m_curTask.readFromBuffer(buffer))
	{
		isValid = false;
		infoStr = "wrong task begin packet";
	}
	else if(!m_taskList.contains(m_curTask.taskName()) || 
		     m_taskList[m_curTask.taskName()]->nInputs() != m_curTask.nInputs())
	{
		isValid = false;
		infoStr = "no such task or wrong number of inputs";
	}
	else
		infoStr = "success";

	qDebug() << infoStr;
	ReplyPacket reply(isValid, infoStr);
	QByteArray  replyBuffer;
	reply.writeToBuffer(replyBuffer);
	isValid &= waitForDataSent(socket, replyBuffer, m_maxSecondsToWait);
	return isValid;
}

bool ExeServer::receiveInput(QTcpSocket& socket)
{
	ExecTask::Ptr pTask = m_taskList[m_curTask.taskName()];
	if (pTask.isNull())
		return false;

	int nInputs = m_curTask.nInputs();
	bool isValid = true;
	for (int ithInput =0; ithInput < nInputs; ++ithInput)
	{
		QByteArray buffer;
		QString infoStr = "OK";
		VariablePacket packet("",pTask.data());
		if (!waitForDataRead(socket, buffer, m_curTask.maxTransferSeconds()))
		{
			isValid = false;
			infoStr = "time out";
		}
		else if (!packet.readFromBuffer(buffer))
		{
			isValid = false;
			infoStr = "wrong input";
		}

		qDebug() << infoStr;
		QByteArray replyBuffer;
		ReplyPacket outPacket(isValid,infoStr);
		outPacket.writeToBuffer(replyBuffer);

		isValid &= waitForDataSent(socket, replyBuffer, m_curTask.maxTransferSeconds());
		if (!isValid)
			break;
	}
	return isValid;
}

bool ExeServer::compute( QTcpSocket& socket )
{
	QByteArray buffer;
	bool isValid = true;
	QString infoStr;
	BeginComputePacket packet;
	ExecTask::Ptr task = m_taskList[m_curTask.taskName()];
	if(!waitForDataRead(socket, buffer, m_curTask.maxTransferSeconds()))
	{
		isValid = false;
		infoStr = "data read error";
	}
	else if(!packet.readFromBuffer(buffer))
	{
		isValid = false;
		infoStr = "wrong task begin packet";
	}
	else 
	{
		isValid = task->compute();
		infoStr += task->getInfo();
		if (isValid)
			infoStr = "computation succeed\n";
		else
			infoStr = "error occurs\n";
	}

	qDebug() << infoStr;
	EndComputePacket reply(isValid, task->nOutputs(), infoStr);
	QByteArray  replyBuffer;
	reply.writeToBuffer(replyBuffer);
	isValid &= waitForDataSent(socket, replyBuffer, m_curTask.maxTransferSeconds());
	return isValid;
}

bool ExeServer::sendOutput( QTcpSocket& socket )
{
	ExecTask::Ptr pTask = m_taskList[m_curTask.taskName()];
	if (pTask.isNull())
		return false;

	int nOutputs = pTask->nOutputs();
	bool isValid = true;
	for (int ithOutput =0; ithOutput < nOutputs; ++ithOutput)
	{
		QByteArray buffer;
		QString infoStr = "OK";
		VariablePacket packet(pTask->getIthOutputName(ithOutput),pTask.data());

		QByteArray replyBuffer;
		ReplyPacket replyPacket;
		if(!packet.writeToBuffer(buffer))
		{
			isValid = false;
			infoStr = "failed to get output";
		}
		else if (!waitForDataSent(socket, buffer, m_curTask.maxTransferSeconds()))
		{
			isValid = false;
			infoStr = "time out";
		}
		else if (!waitForDataRead(socket, replyBuffer, m_curTask.maxTransferSeconds()) ||
			     !replyPacket.readFromBuffer(replyBuffer) ||
				 !replyPacket.isValid())
		{
			isValid = false;
			infoStr = "time out / read error / wrong data";
		}
		else
		{
			infoStr = "ok";
		}

		qDebug() << infoStr;
		if (!isValid)
			break;
	}

	return isValid;
}

bool ExeServer::taskEnd( QTcpSocket& socket )
{
	TaskEndPacket endPacket;
	QByteArray    endBuffer;
	bool isValid = endPacket.writeToBuffer(endBuffer);
	if (isValid)
		isValid &= waitForDataSent(socket, endBuffer, m_curTask.maxTransferSeconds());
	return isValid;
}


void ExecTask::addInputSlot( const QString& slotName )
{
	m_inputSlotList[slotName] = MatrixSlot(slotName);
}

void ExecTask::addOutputSlot( const QString& slotName )
{
	m_outputSlotList[slotName] = MatrixSlot(slotName);
}

bool ExecTask::readInputFromStream( QDataStream& dataStream )
{
	quint32 nInputs;
	dataStream >> nInputs;
	for (unsigned ithInput = 0; ithInput < nInputs; ++ithInput)
	{
		QString slotName;
		dataStream >> slotName;
		if (m_inputSlotList.contains(slotName))
		{
			if(!m_inputSlotList[slotName].readMatrixFromStream(dataStream))
				return false;
		}
	}
	return true;
}

bool ExecTask::writeOutputToStream( QDataStream& dataStream )
{
	quint32 nOutputs = m_outputSlotList.size();
	dataStream << nOutputs;
	QHash<QString, MatrixSlot>::iterator pOut;
	for (pOut = m_outputSlotList.begin(); pOut != m_outputSlotList.end(); ++pOut)
	{
		dataStream << pOut.key();
		if(!pOut.value().writeMatrixToStream(dataStream))
			return false;
	}
	return true;
}

MatrixData* ExecTask::getInputData( const QString& varName )
{
	if (m_inputSlotList.contains(varName))
		return &(m_inputSlotList[varName].getMatData());
	return NULL;
}

MatrixData* ExecTask::getOutputData( const QString& varName )
{
	if (m_outputSlotList.contains(varName))
		return &(m_outputSlotList[varName].getMatData());
	return NULL;
}

ExecTask::ExecTask(const QString& name):m_taskName(name)
{
}

QString ExecTask::getTaskInfo()const
{
	QString str;
	str += "task name: " + m_taskName + "\n";
	str += "input slots:\n";
	for (QHash<QString, MatrixSlot>::ConstIterator pInput = m_inputSlotList.constBegin();
		pInput != m_inputSlotList.constEnd(); ++pInput)
	{
		str += pInput.key() + "\n";
	}

	str += "\noutput slots:\n";
	for (QHash<QString, MatrixSlot>::ConstIterator pOutput = m_outputSlotList.constBegin();
		pOutput != m_outputSlotList.constEnd(); ++pOutput)
	{
		str += "\t" + pOutput.key() + "\n";
	}

	return str;
}


void ExecTask::MatrixSlot::getExpectedSize( int& row, int& col )
{
	row = m_expectedRowSize; col = m_expectedColSize;
}

void ExecTask::MatrixSlot::setExpectedSize( int row, int col )
{
	m_expectedColSize = col; m_expectedRowSize = row;
}

bool ExecTask::MatrixSlot::isValid()
{
	size_t rows = m_data.getRows();
	size_t cols = m_data.getCols();
	if ((m_expectedRowSize!=-1 && rows !=m_expectedRowSize) || rows == 0)
		return false;
	if ((m_expectedColSize!=-1 && cols !=m_expectedColSize) || cols == 0)
		return false;
	if (!(m_expectedDataType & m_data.getDataType()))
		return false;
	if (!(m_expectedMatType  & m_data.getMatrixType()))
		return false;
	return true;
}

ExecTask::MatrixSlot::MatrixSlot(QString varName, 
								 int rowSize /*= -1*/, 
								 int colSize /*= -1*/, 
								 MatrixData::MatrixType matType /*= MatrixData::MT_ALL*/, 
								 MatrixData::DataType dataType /*= MatrixData::DT_ALL*/ ):m_data()
{
	m_varName = varName;
	m_expectedRowSize = rowSize;
	m_expectedColSize = colSize;
	m_expectedDataType = dataType;
	m_expectedMatType  = matType;
}

bool ExecTask::MatrixSlot::readMatrixFromStream( QDataStream& dataStream )
{
	dataStream >> m_data;
	return isValid();
}

bool ExecTask::MatrixSlot::writeMatrixToStream( QDataStream& dataStream )
{
	if (!isValid())
		return false;
	dataStream << m_data;
	return true;
}

ExecTask::MatrixSlot& ExecTask::MatrixSlot::operator=( const MatrixSlot& src )
{
	m_varName = src.m_varName;
	m_expectedColSize = src.m_expectedColSize;
	m_expectedRowSize = src.m_expectedRowSize;
	m_expectedDataType= src.m_expectedDataType;
	m_expectedMatType = src.m_expectedMatType;
	m_data = src.m_data;
	return *this;
}

