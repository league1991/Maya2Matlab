#pragma once

class ExePacket
{
public:
	enum PacketType
	{
		TASK_BEGIN   = 0,
		TASK_END     = 1,
		REPLY        = 2,
		VARIABLE     = 3,
		COMPUTE_BEGIN= 4,
		COMPUTE_END  = 5
	};
	ExePacket(void);
	~ExePacket(void);

	virtual bool readFromBuffer(QByteArray&  byte) = 0;
	virtual bool writeToBuffer(QByteArray&  byte) = 0;
protected:
	bool		readHeaderFromDS( QDataStream&  ds , PacketType type);
	bool		writeHeaderToDS ( QDataStream&  ds , PacketType type);

private:
	const static QString m_exePacketHeader;
};

class TaskBeginPacket:public ExePacket
{
public:	
	TaskBeginPacket(const QString& taskName = "", 
		            quint32 nInputs = 0,
					quint32 maxTransferSeconds = 0,
					quint32 maxComputeSeconds  = 0);

	QString     taskName(){return m_taskName;}
	quint32     nInputs(){return m_nInputs;}
	quint32     maxTransferSeconds(){return m_maxTransferSeconds;}
	quint32     maxComputeSeconds(){return m_maxComputeSeconds;}

	bool		readFromBuffer(QByteArray&  byte);
	bool        writeToBuffer(QByteArray&  byte);
private:
	QString              m_taskName;
	quint32	             m_nInputs;
	quint32              m_maxTransferSeconds;
	quint32			     m_maxComputeSeconds;
};

class TaskEndPacket: public ExePacket
{
public:
	bool		readFromBuffer(QByteArray&  byte);
	bool        writeToBuffer(QByteArray&  byte);
protected:
};

class ReplyPacket:public ExePacket
{
public:
	ReplyPacket(bool isValid = false, const QString& info = "");
	bool		readFromBuffer(QByteArray&  byte);
	bool        writeToBuffer(QByteArray&  byte);
	bool        isValid(){return m_isValid != 0;}
	QString		getInfo(){return m_info;}
protected:
	quint32     m_isValid;
	QString     m_info;
};

class ExecTask;
class VariablePacket:public ExePacket
{
public:
	VariablePacket(QString varName, MatrixData* data):m_varName(varName),m_pMat(data),m_pTask(NULL),m_isMatFromTask(false){}
	VariablePacket(QString varName, ExecTask* pTask):m_varName(varName),m_pMat(NULL),m_pTask(pTask),m_isMatFromTask(true){}

	QString     variableName(){return m_varName;}

	bool		readFromBuffer(QByteArray&  byte);
	bool        writeToBuffer (QByteArray&  byte);

	bool        isValid(){return m_varName.trimmed() != "" && m_pMat;}
private:
	bool        m_isMatFromTask;
	ExecTask*   m_pTask;
	QString     m_varName;
	MatrixData* m_pMat;
};

class BeginComputePacket:public ExePacket
{
public:
	BeginComputePacket(quint32 maxComputeSec = 10){m_maxComputeSec = maxComputeSec;}
	bool		readFromBuffer(QByteArray&  byte);
	bool        writeToBuffer(QByteArray&  byte);
	quint32     maxComputeSeconds(){return m_maxComputeSec;}
private:
	quint32     m_maxComputeSec;
};

class EndComputePacket:public ExePacket
{
public:
	EndComputePacket(quint32 isSuccess = false, quint32 nOutputs = 0, const QString& info = "");
	bool		readFromBuffer(QByteArray&  byte);
	bool        writeToBuffer(QByteArray&  byte);
	bool        isSuccess(){return m_isSuccess != 0;}
	quint32     nOutputs(){return m_nOutput;}
	QString     info(){return m_info;}
protected:
	QString     m_info;
	quint32     m_isSuccess;
	quint32     m_nOutput;
};