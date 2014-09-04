#pragma once


class ExeClientNode: public MPxLocatorNode
{
public:
	ExeClientNode(void);
	virtual ~ExeClientNode(void);

	virtual void draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView:: DisplayStatus );
	virtual bool isBounded() const{return true;}
	virtual MBoundingBox boundingBox() const;

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	static  void*		creator(){return new ExeClientNode;}
	static  MStatus		initialize();

	MStatus             computeExe();

	// The typeid is a unique 32bit indentifier that describes this node.
	// It is used to save and retrieve nodes of this type from the binary
	// file format.  If it is not unique, it will cause file IO problems.
	//
	static	MTypeId		m_id;
	static const char*  m_nodeName;

private:
	void                drawIcon();
	void				drawText();
	int                 numInputs();
	MStatus             getOutputIDMap(QHash<QString,int>& outputMap);
	void				buildNodeShape( const MString& name );
	static MObject      m_inputMatrixArray;							
	static MObject      m_outputMatrixArray;
	static MObject      m_inputIdentifierArray;
	static MObject      m_outputIdentifierArray;

	static MObject      m_exePath;
	static MObject      m_resultString;
	static MObject      m_immediatelyUpdate;
	static MObject      m_maxComputeSeconds;
	static MObject      m_maxTransferSeconds;
	static MObject      m_taskIdentifier;
	static MObject      m_ipAddress;
	static MObject      m_port;

	static const char*  m_inputMatrixArrayName[2];
	static const char*  m_outputMatrixArrayName[2];
	static const char*  m_inputIdentifierArrayName[2];
	static const char*  m_outputIdentifierArrayName[2];

	static const char*  m_resultStringName[2];
	static const char*  m_exePathName[2];
	static const char*  m_immediatelyUpdateName[2];
	static const char*  m_maxComputeSecondsName[2];
	static const char*  m_maxTransferSecondsName[2];
	static const char*  m_taskIdentifierName[2];
	static const char*  m_ipAddressName[2];
	static const char*  m_portName[2];

	static const float  m_exeIconCurve[];
	QPolygonF           m_textShape;
	// node object name
	MString             m_objectName;
	MBoundingBox        m_box;
};
