
#include "stdafx.h"
#include "serverstdafx.h"
#include "ClientStdAfx.h"
#include "ExePacket.h"

const QString ExePacket::m_exePacketHeader = "Exe Engine Header";


ExePacket::ExePacket(void)
{
}

ExePacket::~ExePacket(void)
{
}

bool ExePacket::readHeaderFromDS( QDataStream& ds , PacketType type)
{
	QString s;
	ds >> s;
	quint32 t;
	ds >> t;	
	return s == m_exePacketHeader && type == (PacketType)t;
}

bool ExePacket::writeHeaderToDS( QDataStream&  ds , PacketType type)
{
	ds << m_exePacketHeader;
	ds << (quint32)type;
	return true;
}

bool TaskBeginPacket::readFromBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::ReadOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!readHeaderFromDS(ds, TASK_BEGIN))
		return false;
	ds >> m_taskName;
	ds >> m_nInputs;
	ds >> m_maxTransferSeconds >> m_maxComputeSeconds;
	return true;
}

bool TaskBeginPacket::writeToBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::WriteOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!writeHeaderToDS(ds, TASK_BEGIN))
		return false;
	if (m_taskName.trimmed() == "" || 
		m_maxComputeSeconds <= 0   || m_maxTransferSeconds <= 0)
		return false;

	ds << m_taskName;
	ds << m_nInputs;
	ds << m_maxTransferSeconds << m_maxComputeSeconds;
	return true;
}

TaskBeginPacket::TaskBeginPacket( const QString& taskName, quint32 nInputs, quint32 maxTransferSeconds, quint32 maxComputeSeconds )
{
	m_taskName = taskName;m_nInputs = nInputs;
	m_maxTransferSeconds = maxTransferSeconds;
	m_maxComputeSeconds = maxComputeSeconds;
}

bool TaskEndPacket::readFromBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::ReadOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!readHeaderFromDS(ds, TASK_END))
		return false;
	return true;
}

bool TaskEndPacket::writeToBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::WriteOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!writeHeaderToDS(ds, TASK_END))
		return false;
	return true;
}

bool ReplyPacket::readFromBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::ReadOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!readHeaderFromDS(ds, REPLY))
		return false;
	ds >> m_isValid;
	ds >> m_info;
	return true;
}

bool ReplyPacket::writeToBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::WriteOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!writeHeaderToDS(ds, REPLY))
		return false;
	ds << m_isValid;
	ds << m_info;
	return true;
}

ReplyPacket::ReplyPacket( bool isValid, const QString& info /*= ""*/ )
{
	m_isValid = (quint32)isValid;
	m_info    = info;
}



bool VariablePacket::readFromBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::ReadOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!readHeaderFromDS(ds, VARIABLE))
		return false;

	ds >> m_varName;
	if (m_varName.trimmed() == "")
		return false;
	
	if (m_isMatFromTask)
		if (m_pTask)
			m_pMat = m_pTask->getInputData(m_varName);
		else
			return false;
	
	if (!m_pMat)
			return false;

	ds >> (*m_pMat);
	if (m_pMat->getRows() <= 0 || m_pMat->getCols() <= 0)
		return false;
	return true;
}

bool VariablePacket::writeToBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::WriteOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (m_varName.trimmed() == "" || !writeHeaderToDS(ds, VARIABLE))
		return false;

	if (m_varName.trimmed() == "")
		return false;
	ds << m_varName;

	if (m_isMatFromTask)
		if (m_pTask)
			m_pMat = m_pTask->getOutputData(m_varName);
		else
			return false;

	if (!m_pMat || m_pMat->getRows() <= 0 || m_pMat->getCols() <= 0)
		return false;

	ds << (*m_pMat);
	return true;
}

bool BeginComputePacket::readFromBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::ReadOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!readHeaderFromDS(ds, COMPUTE_BEGIN))
		return false;
	ds >> m_maxComputeSec;
	return m_maxComputeSec != 0;
}

bool BeginComputePacket::writeToBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::WriteOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!writeHeaderToDS(ds, COMPUTE_BEGIN))
		return false;
	ds << m_maxComputeSec;
	return true;
}

EndComputePacket::EndComputePacket( quint32 isSuccess, quint32 nOutputs, const QString& info /*= ""*/ )
{
	m_info = info;
	m_isSuccess = isSuccess;
	m_nOutput = nOutputs;
}

bool EndComputePacket::readFromBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::ReadOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!readHeaderFromDS(ds, COMPUTE_END))
		return false;
	ds >> m_isSuccess;
	ds >> m_nOutput;
	ds >> m_info;
	return true;
}

bool EndComputePacket::writeToBuffer( QByteArray&  byte )
{
	QDataStream ds(&byte, QIODevice::WriteOnly);
	ds.setVersion(DATA_STREAM_VERSION);
	if (!writeHeaderToDS(ds, COMPUTE_END))
		return false;
	ds << m_isSuccess << m_nOutput << m_info;
	return true;
}
