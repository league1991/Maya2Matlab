#pragma once

#define DATA_PACKET_HEADER    "MatlabPlugin Node Data Header"
#define ERROR_PACKET_HEADER   "MatlabPlugin Error Header"

class ExecTask
{
public:
	typedef QSharedPointer<ExecTask> ExecTask::Ptr;

	// you should add input and output slots by calling addInputSlot/addOutputSlot function
	ExecTask(const QString& name = "defaultTask");

	// return this task's name, which should be unique
	const QString& name(){return m_taskName;}

	// compute function
	virtual bool compute()=0;

	bool readInputFromStream(QDataStream& dataStream);
	bool writeOutputToStream(QDataStream& dataStream);

	QString getInfo(){return m_infoBuffer;}

	unsigned  nInputs(){return m_inputSlotList.size();}
	unsigned  nOutputs(){return m_outputSlotList.size();}	

	MatrixData* getInputData(const QString& varName);
	MatrixData* getOutputData(const QString& varName);

	QString   getIthOutputName(int ithOut){return m_outputSlotList.keys()[ithOut];}

	QString   getTaskInfo()const;
protected:
	class MatrixSlot
	{
	public:
		MatrixSlot():m_data(){}

		MatrixSlot(QString varName, 
			       int rowSize = -1, int colSize = -1, 
				   MatrixData::MatrixType matType  = MatrixData::MT_ALL,
				   MatrixData::DataType   dataType = MatrixData::DT_ALL);
		const QString& getVarName(){return m_varName;}
		MatrixData&    getMatData(){return m_data;}

		void           getExpectedSize(int& row, int& col);
		void           setExpectedSize(int row, int col);

		bool           readMatrixFromStream(QDataStream& dataStream);
		bool           writeMatrixToStream(QDataStream& dataStream);

		bool           isValid();
		MatrixSlot&    operator=(const MatrixSlot& src);
	private:
		QString m_varName;
		int     m_expectedRowSize;                  // variable used to check if row/column size is valid
		int     m_expectedColSize;					// if the size isn't limitted, set to -1
		MatrixData::DataType   m_expectedDataType;  // data type
		MatrixData::MatrixType m_expectedMatType;   // matrix type (dense/sparse)
													
		MatrixData m_data;
	};


	// add input slots
	void addInputSlot (const QString& slotName);
	void addOutputSlot(const QString& slotName);

	QString                    m_infoBuffer;
private:
	QString                    m_taskName;
	QHash<QString, MatrixSlot> m_inputSlotList;
	QHash<QString, MatrixSlot> m_outputSlotList;
};

class ExeServer:public QThread
{
public:
	ExeServer(void);
	~ExeServer(void);

	// add new task, the task should be preallocated
	void addTask(ExecTask::Ptr task);		
	void setMaxSecondsToWait(int maxSecondsToWait);
	void setPort(quint16 port);

protected:
	void run();
private slots:
	//void getNewConnection();

private:
	void printServerInfo();
	bool waitForDataRead(QTcpSocket& socket, QByteArray& data, const int maxSeconds /*= 10*/ );
	bool waitForDataSent(QTcpSocket& socket, const QByteArray& data , const int maxSeconds);
	
	bool taskBegin(QTcpSocket& socket);
	bool receiveInput(QTcpSocket& socket);
	bool compute(QTcpSocket& socket);
	bool sendOutput(QTcpSocket& socket);
	bool taskEnd(QTcpSocket& socket);

	void sendErrorReport(QTcpSocket& socket, const QString & errorString);

	QTcpServer               m_server;
	quint16                  m_port;
	QHash<QString, ExecTask::Ptr> m_taskList;
	int                      m_maxSecondsToWait;
	TaskBeginPacket          m_curTask;
};
