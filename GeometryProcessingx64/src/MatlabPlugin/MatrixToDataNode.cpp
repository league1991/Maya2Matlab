#include "StdAfx.h"
#include "MatrixToDataNode.h"

const char* MatrixToDataNode::m_nodeName = NODE_MATRIX_TO_DATA_NAME;
MTypeId     MatrixToDataNode::m_id(NODE_MATRIX_TO_DATA_ID);


const char* MatrixToDataNode::m_outMatrixName[2] ={"outMatrix", "oMat"};
const char* MatrixToDataNode::m_inMatrixName[2] = {"inMatrix",  "iMat"};
const char* MatrixToDataNode::m_outPointName[2] = {"outPoints", "oPnts"};
const char* MatrixToDataNode::m_inPointName[2]  = {"inPoints",  "iPnts"};

MObject MatrixToDataNode::m_outPoint;
MObject MatrixToDataNode::m_inPoint;
MObject MatrixToDataNode::m_inMatrix;
MObject MatrixToDataNode::m_outMatrix;

MatrixToDataNode::MatrixToDataNode(void)
{
}

MatrixToDataNode::~MatrixToDataNode(void)
{
}

MStatus MatrixToDataNode::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus s;
	if (plug == m_outPoint || plug.parent() == m_outPoint)
	{
		MPlug inPointsPlugArray  = Global::getPlug(this, m_inPointName[0]);
		MPlug outPointsPlugArray = Global::getPlug(this, m_outPointName[0]);
		for (unsigned idxPhy = 0; idxPhy < outPointsPlugArray.numElements(); ++idxPhy)
		{
			MPlug outPointsPlug = outPointsPlugArray.elementByPhysicalIndex(idxPhy, &s);
			if (!s)
				continue;

			int logicalIdx = outPointsPlug.logicalIndex(&s);
			MPlug inPointsPlug = inPointsPlugArray.elementByLogicalIndex(logicalIdx, &s);

			MObject inPointsObj = inPointsPlug.asMObject();
			MatlabMatrix* pMat;
			s = MatlabMatrix::checkAndGetMatrixPtr(inPointsObj, pMat, MatrixData::MT_DENSE);
			if (!s)
				continue;

			size_t r,c;
			pMat->getMatrix().getDimension(r,c);
			bool isVaild = (r == 1 && c > 0 && c < 4) || (c == 1 && r > 0 && r < 4);
			if (!isVaild)
			{
				Global::displayError(this, "Matrix must be vector with 1/2/3 components.");
				continue;
			}

			double pData[3] = {0.0};
			pMat->getMatrix().getDenseDataDouble(pData);

			MFnNumericData    outPointsFn;
			MObject outPointsObj = outPointsFn.create(MFnNumericData::k3Double, &s);
			if (s)
			{
				outPointsFn.setData3Double(pData[0], pData[1], pData[2]);
				outPointsPlug.setValue(outPointsObj);
			}
		}
	}
	else if (plug == m_outMatrix || plug.parent() == m_outMatrix)
	{
		MPlug inMatrixPlugArray  = Global::getPlug(this, m_inMatrixName[0]);
		MPlug outMatrixPlugArray = Global::getPlug(this, m_outMatrixName[0]);
		for (unsigned idxPhy = 0; idxPhy < outMatrixPlugArray.numElements(); ++idxPhy)
		{
			MPlug outMatrixPlug = outMatrixPlugArray.elementByPhysicalIndex(idxPhy, &s);
			if (!s)
				continue;

			int logicalIdx = outMatrixPlug.logicalIndex(&s);
			MPlug inMatrixPlug = inMatrixPlugArray.elementByLogicalIndex(logicalIdx, &s);

			MObject inMatrixObj = inMatrixPlug.asMObject();
			MatlabMatrix* pMat;
			s = MatlabMatrix::checkAndGetMatrixPtr(inMatrixObj, pMat, MatrixData::MT_DENSE);
			if (!s)
				continue;

			size_t r,c;
			pMat->getMatrix().getDimension(r,c);
			bool isVaild = (r == 4 && c == 4);
			if (!isVaild)
			{
				Global::displayError(this, "Matrix must be 4x4.");
				continue;
			}

			double pData[4][4] = {0.0};
			pMat->getMatrix().getDenseDataDouble((double*)pData);

			MMatrix mat(pData);
			mat = mat.transpose();
			MFnMatrixData    outMatrixFn;
			MObject outMatrixObj = outMatrixFn.create(mat, &s);
			if (s)
			{
				outMatrixPlug.setValue(outMatrixObj);
			}
		}
	}
	data.setClean(plug);
	return s;
}

MStatus MatrixToDataNode::initialize()
{
	MStatus s; 
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute  mAttr;

	m_inPoint = tAttr.create(m_inPointName[0], m_inPointName[1], MFnData::kPlugin, &s);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setArray(true);
	tAttr.setUsesArrayDataBuilder(true);

	m_outPoint =  nAttr.create(m_outPointName[0], m_outPointName[1], MFnNumericData::k3Double, 0.0, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setReadable(true);
	nAttr.setArray(true);
	nAttr.setUsesArrayDataBuilder(true);


	m_inMatrix = tAttr.create(m_inMatrixName[0], m_inMatrixName[1], MFnData::kPlugin, &s);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setArray(true);
	tAttr.setUsesArrayDataBuilder(true);

	m_outMatrix =  mAttr.create(m_outMatrixName[0], m_outMatrixName[1], MFnMatrixAttribute::kDouble);
	mAttr.setStorable(true);
	mAttr.setHidden(false);
	mAttr.setWritable(false);
	mAttr.setReadable(true);
	mAttr.setArray(true);
	mAttr.setUsesArrayDataBuilder(true);

	CHECK_STATUS(s);
	s = addAttribute( m_inPoint);
	CHECK_STATUS(s);
	s = addAttribute( m_outPoint);
	CHECK_STATUS(s);
	s = addAttribute( m_inMatrix);
	CHECK_STATUS(s);
	s = addAttribute( m_outMatrix);
	CHECK_STATUS(s);

	s = attributeAffects(m_inPoint, m_outPoint);
	CHECK_STATUS(s);
	s = attributeAffects(m_inMatrix, m_outMatrix);
	CHECK_STATUS(s);

	return MS::kSuccess;
}


