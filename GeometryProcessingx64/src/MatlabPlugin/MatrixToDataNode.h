#pragma once

class MatrixToDataNode:public MPxNode
{
public:
	MatrixToDataNode(void);
	~MatrixToDataNode(void);

	static void*		creator(){return new MatrixToDataNode;}
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	static  MStatus		initialize();

	static MTypeId		m_id;
	static const char*  m_nodeName;
private:

	static MObject m_inPoint;
	static MObject m_outPoint;
	static MObject m_inMatrix;
	static MObject m_outMatrix;

	static const char* m_inPointName[2];
	static const char* m_outPointName[2];
	static const char* m_inMatrixName[2];
	static const char* m_outMatrixName[2];
};
