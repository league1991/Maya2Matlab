#pragma once

class DataToMatrixNode :
	public MPxNode
{
public:
	DataToMatrixNode(void);
	~DataToMatrixNode(void);

	static void*		creator(){return new DataToMatrixNode;}
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	static  MStatus		initialize();

	static MTypeId		m_id;
	static const char*  m_nodeName;
private:

	static MObject m_inScalar;
	static MObject m_inMatrix;
	static MObject m_inPoint;

	static MObject m_outScalar;
	static MObject m_outMatrix;
	static MObject m_outPoint;

	static const char* m_inScalarName[2];
	static const char* m_inMatrixName[2];
	static const char* m_inPointName[2];
	static const char* m_curIdxName[2];

	static const char* m_outScalarName[2];
	static const char* m_outMatrixName[2];
	static const char* m_outPointName[2];
};
