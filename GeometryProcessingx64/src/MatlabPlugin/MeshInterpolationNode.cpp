#include "StdAfx.h"
#include "MeshInterpolationNode.h"

MeshInterpolationNode::MeshInterpolationNode(void)
{
}

MeshInterpolationNode::~MeshInterpolationNode(void)
{
}

const char* MeshInterpolationNode::m_outMeshName[2]={"outMesh","out"};

const char* MeshInterpolationNode::m_nKeyFramesName[2]={"numKeyFrames","nKeys"};

const char* MeshInterpolationNode::m_vertexMatricesName[2]={"vertexMatrices","vertMat"};

const char* MeshInterpolationNode::m_weightName[2]={"weight","weight"};

const char* MeshInterpolationNode::m_inMeshName[2]={"inMesh","in"};

MObject MeshInterpolationNode::m_outMesh;
MObject MeshInterpolationNode::m_nKeyFrames;
MObject MeshInterpolationNode::m_vertexMatrices;
MObject MeshInterpolationNode::m_weight;
MObject MeshInterpolationNode::m_inMesh;

const char* MeshInterpolationNode::m_nodeName = NODE_MESH_INTERPOLATION_NAME;

MTypeId MeshInterpolationNode::m_id(NODE_MESH_INTERPOLATION_ID);


MStatus MeshInterpolationNode::initialize()
{
	MStatus s;

	// add instruction attribute
	MFnTypedAttribute   tAttr;
	MFnNumericAttribute nAttr;

	m_inMesh = tAttr.create(m_inMeshName[0], m_inMeshName[1], MFnData::kMesh, &s);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(true);
	tAttr.setKeyable(false);


	m_outMesh = tAttr.create(m_outMeshName[0], m_outMeshName[1], MFnData::kMesh, &s);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(true);
	tAttr.setKeyable(false);

	m_vertexMatrices = tAttr.create(m_vertexMatricesName[0], m_vertexMatricesName[1], MFnData::kPlugin,&s);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setKeyable(false);


	m_weight = nAttr.create(m_weightName[0], m_weightName[1], MFnNumericData::kDouble,0,&s);
	nAttr.setKeyable(true);
	nAttr.setSoftMin(0);
	nAttr.setSoftMax(1);


	m_nKeyFrames = nAttr.create(m_nKeyFramesName[0], m_nKeyFramesName[1], MFnNumericData::kInt,0,&s);
	nAttr.setKeyable(false);
	nAttr.setWritable(false);
	nAttr.setSoftMin(0);
	nAttr.setSoftMax(1);

	s = addAttribute(m_inMesh);
	CHECK_STATUS(s);
	s = addAttribute(m_outMesh);
	CHECK_STATUS(s);
	s = addAttribute(m_vertexMatrices);
	CHECK_STATUS(s);
	s = addAttribute(m_nKeyFrames);
	CHECK_STATUS(s);
	s = addAttribute(m_weight);
	CHECK_STATUS(s);

	s = attributeAffects(m_inMesh, m_outMesh);
	CHECK_STATUS(s);
	s = attributeAffects(m_weight, m_outMesh);
	CHECK_STATUS(s);
	s = attributeAffects(m_vertexMatrices, m_outMesh);
	CHECK_STATUS(s);
	s = attributeAffects(m_vertexMatrices, m_nKeyFrames);
	CHECK_STATUS(s);

	return s ;
}

MStatus MeshInterpolationNode::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus s;

	// get vertex matrices data
	MPlug       vertMatPlug = Global::getPlug(this, m_vertexMatricesName[0]);
	MObject		vertMatObj;
	MatlabMatrix* pVertMat;
	vertMatPlug.getValue(vertMatObj);
	s = MatlabMatrix::checkAndGetMatrixPtr(vertMatObj, pVertMat, 
										MatrixData::MT_DENSE, MatrixData::DT_DOUBLE);

	CHECK_STATUS(s);

	if (plug == m_outMesh)
	{
		// retrieve mesh data
		MDataHandle inMeshHnd = data.inputValue(m_inMesh);
		MObject     inMeshObj  = inMeshHnd.asMesh();
		MFnMeshData meshData;
		MObject     outMeshObj = meshData.create();

		// copy data to output mesh
		MFnMesh     outMeshFn;
		outMeshFn.copy(inMeshObj, outMeshObj);
		outMeshFn.setObject(outMeshObj);
		int nVtx = outMeshFn.numVertices(&s);

		// get matrix data and check them
		double* pVertBuffer;
		size_t nRows, nCols;
		MatrixData::DataType type;
		pVertMat->getMatrix().getDenseDataPtr((void*&)pVertBuffer, nRows, nCols, type);
		if (nRows != nVtx || nCols % 3 != 0)
		{
			MGlobal::displayError(this->name() + MString(": Matrix dimension wrong. Should have nVtx rows and nFrames * 3 cols."));
			return MS::kFailure;
		}

		// get weight and calculate interpolation weights
		MPlug       weightPlug = Global::getPlug(this, m_weightName[0]);
		double      weightValue;
		weightPlug.getValue(weightValue);

		size_t nKeyFrames = nCols/3;
		weightValue *= (nKeyFrames -1);
		int floorFrame = CLAMP_INT(0, nKeyFrames-1, floor(weightValue));
		int	ceilFrame  = CLAMP_INT(0, nKeyFrames-1, ceil (weightValue));

		double floorWeight = ceilFrame == floorFrame ? 1.0 : ceilFrame   - weightValue;
		double ceilWeight  = ceilFrame == floorFrame ? 0.0 : weightValue - floorFrame;

		double* floorPtr = pVertBuffer + nVtx * 3 * floorFrame;
		double* ceilPtr  = pVertBuffer + nVtx * 3 * ceilFrame;
		const int nVtx2  = nVtx * 2;
		MItMeshVertex it(outMeshObj, &s);
		for (int ithVtx = 0; !it.isDone(&s); it.next(), ++ithVtx, ++floorPtr, ++ceilPtr)
		{
			MPoint p(
				floorPtr[0]    * floorWeight + ceilPtr[0]    * ceilWeight,
				floorPtr[nVtx] * floorWeight + ceilPtr[nVtx] * ceilWeight,
				floorPtr[nVtx2]* floorWeight + ceilPtr[nVtx2]* ceilWeight,1.0);
			it.setPosition(p);
		}

		// set output mesh
		data.outputValue(m_outMesh).set(outMeshObj);
	}

	// set nKeyframes
	data.outputValue(m_nKeyFrames).setInt(pVertMat->getMatrix().getCols() / 3);
	data.setClean(plug);
	return s;
}
