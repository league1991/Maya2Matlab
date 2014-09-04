#pragma once
#include "stdafx.h"

class MeshInterpolationNode:public MPxNode
{
public:
	MeshInterpolationNode(void);
	virtual ~MeshInterpolationNode(void);

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	static  void*		creator(){return new MeshInterpolationNode;}
	static  MStatus		initialize();

	static MTypeId		m_id;
	static const char*  m_nodeName;

	static MObject		m_inMesh;
	static MObject		m_weight;
	// vertex matrices
	// (nVtx, 3 * nFrame) 
	// [frame1.x frame1.y frame1.z  ...  frameN.x frameN.y frameN.z]
	static MObject		m_vertexMatrices;
	static MObject		m_nKeyFrames;
	static MObject		m_outMesh;

private:
	static const char*  m_inMeshName[2];
	static const char*  m_weightName[2];
	static const char*	m_vertexMatricesName[2];
	static const char*  m_nKeyFramesName[2];
	static const char*	m_outMeshName[2];
};
