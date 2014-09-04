#pragma once
#include "stdafx.h"
class MeshModifierNode:public MPxNode
{
public:
	enum ColorSource{FROM_VERTEX_VALUE = 0, FROM_COLOR_ARRAY = 1};
	MeshModifierNode(void);
	virtual  ~MeshModifierNode(void);

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator(){return new MeshModifierNode;}
	static  MStatus		initialize();

	static MTypeId		m_id;
	static const char*  m_nodeName;
private:
	MStatus				modifyVertexColor(MObject&outMeshObj);
	MStatus				modifyVertexNormal(MObject&outMeshObj);
	MStatus				modifyVertexPosition(MObject&outMeshObj);

	static MObject		m_inMesh;
	static MObject		m_outMesh;
	static MObject		m_autoRange;
	static MObject		m_minValue;
	static MObject		m_maxValue;
	static MObject		m_vertValArray;
	static MObject		m_norDirArray;
	static MObject		m_vertPosArray;
	static MObject      m_vertColorArray;
	static MObject      m_vertColorSource;


	static const char*  m_inMeshName[2];
	static const char*  m_outMeshName[2];
	static const char*  m_autoRangeName[2];
	static const char*  m_minValueName[2];
	static const char*  m_maxValueName[2];
	static const char*  m_vertValArrayName[2];
	static const char*  m_norDirArrayName[2];
	static const char*  m_vertPosArrayName[2];
	static const char*  m_vertColorArrayName[2];
	static const char*  m_vertColorSourceName[2];


	static const unsigned char m_colorMap0[300];
	static const unsigned char m_colorMap1[300];
};
