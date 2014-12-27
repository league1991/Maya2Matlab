#pragma once
#include "stdafx.h"

class MeshToMatrixNode :
	public MPxNode
{
public:
					MeshToMatrixNode(void);
	virtual			~MeshToMatrixNode(void);
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:

	// There needs to be a MObject handle declared for each attribute that
	// the node will have.  These handles are needed for getting and setting
	// the values later.
	//
	static  MObject     m_inMesh;				// input mesh
	static  MObject     m_space;				// world/local space
	static  MObject     m_transformMatrix;		// transform matrix
	static  MObject     m_vertexMatrix;			// output matrix of vertex positions
	static  MObject     m_normalMatrix;         // output matrix of normal directions
	static  MObject     m_tangentMatrix;        // output matrix of tangent directions
	static  MObject     m_bitangentMatrix;      // output matrix of bitangent directions
	static  MObject		m_minCurDirMatrix;		// min curvature direction
	static  MObject		m_maxCurDirMatrix;		// max curvature direction
	static  MObject		m_minCurMatrix;			// min curvature
	static  MObject		m_maxCurMatrix;			// max curvature
	static  MObject		m_adjacencyMatrix;		// adjacency matrix from vertices
	static  MObject		m_VEIncidenceMatrix;		// output incident matrix of vertices and edges (sparse)
	static  MObject		m_VFIncidenceMatrix;		// output incident matrix of vertices and faces
	static  MObject		m_cotLaplacianMatrix;		// cotangent Laplacian matrix
	static  MObject		m_voronoiArea;				// voronoi area of each point
	//static  MObject		m_EFIncidentMatrix;		// output incident matrix of edges    and faces

	// The typeid is a unique 32bit indentifier that describes this node.
	// It is used to save and retrieve nodes of this type from the binary
	// file format.  If it is not unique, it will cause file IO problems.
	//
	static	MTypeId		m_id;
	static  const char* m_nodeName;

private:
	MPlug				getPlug(const char* longName);
	MStatus				computeVertexMatrix(MObject& meshObj);
	MStatus				computeVEIncidenceMatrix(MObject& meshObj);
	MStatus				computeVFIncidenceMatrix(MObject& meshObj);
	MStatus				computeAdjacencyMatrix(MObject& meshObj);
	MStatus				computePrincipleCurvature(MObject& meshObj);
	MStatus             computeLocalFrameMatrix( MObject& mesh );		// compute normal, tangent, bitangent
	MStatus				computeCotLaplacian(MObject& mesh);
	MSpace::Space       getCurrentSpace();
	MMatrix				getTransMat();
	inline MPoint		getMeshPoint(int ithVtx, const MFnMesh& meshFn, const MMatrix& transMat, MSpace::Space space)
	{
		MPoint pos;
		meshFn.getPoint(ithVtx, pos, space);
		return pos * transMat;
	}

	static const char*  m_inMeshName[2];
	static const char*  m_spaceName[2];
	static const char*  m_transformMatrixName[2];
	static const char*  m_vertexMatrixName[2];
	static const char*  m_normalMatrixName[2];
	static const char*  m_tangentMatrixName[2];
	static const char*  m_bitangentMatrixName[2];
	static const char*  m_minCurDirMatrixName[2];
	static const char*  m_maxCurDirMatrixName[2];
	static const char*  m_minCurMatrixName[2];
	static const char*  m_maxCurMatrixName[2];
	static const char*  m_adjacencyMatrixName[2];
	static const char*  m_VEIncidenceMatrixName[2];
	static const char*  m_VFIncidenceMatrixName[2];
	static const char*  m_cotLaplacianMatrixName[2];
	static const char*	m_voronoiAreaName[2];
	//static const char*  m_EFIncidentMatrixName[2];
};
