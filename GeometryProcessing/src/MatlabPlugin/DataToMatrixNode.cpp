#include "StdAfx.h"
#include "DataToMatrixNode.h"

const char* DataToMatrixNode::m_nodeName = NODE_DATA_TO_MATRIX_NAME;

MTypeId DataToMatrixNode::m_id(NODE_DATA_TO_MATRIX_ID);
const char* DataToMatrixNode::m_outScalarName[2]={"outScalar", "outScl"};
const char* DataToMatrixNode::m_outMatrixName[2]={"outMatrix", "outMat"};
const char* DataToMatrixNode::m_outPointName[2] ={"outPoint",  "outPnt"};

const char* DataToMatrixNode::m_inPointName[2] ={"inPoint", "inPnt"};
const char* DataToMatrixNode::m_inScalarName[2]={"inScalar","inScl"};
const char* DataToMatrixNode::m_inMatrixName[2]={"inMatrix","inMat"};
const char* DataToMatrixNode::m_curIdxName[2]  ={"currentIndex", "curIdx"};

MObject DataToMatrixNode::m_inPoint;
MObject DataToMatrixNode::m_outScalar;
MObject DataToMatrixNode::m_outMatrix;

MObject DataToMatrixNode::m_outPoint;
MObject DataToMatrixNode::m_inScalar;
MObject DataToMatrixNode::m_inMatrix;

DataToMatrixNode::DataToMatrixNode(void)
{
	
}

DataToMatrixNode::~DataToMatrixNode(void)
{
}
MStatus DataToMatrixNode::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus s;
	if (plug == m_outScalar || plug.parent() == m_outScalar)
	{
		MPlug outScalarPlugArray = Global::getPlug(this, m_outScalarName[0]);
		for (unsigned idxPhy = 0; idxPhy < outScalarPlugArray.numElements(); ++idxPhy)
		{
			MPlug outScalarPlug = outScalarPlugArray.elementByPhysicalIndex(idxPhy, &s);
			CHECK_STATUS(s)
			if (!outScalarPlug.isConnected())
				continue; 
			int logicalIdx = outScalarPlug.logicalIndex(&s);
			MPlug inScalarPlugArray = Global::getPlug(this, m_inScalarName[0]);
			MPlug inScalarPlug = inScalarPlugArray.elementByLogicalIndex(logicalIdx, &s);
			CHECK_STATUS(s)
			double inScalarValue = inScalarPlug.asDouble();
			MObject outScalarObj;
			MatlabMatrix* outScalarPtr;
			if (MatlabMatrix::createMatrixObject(outScalarObj, outScalarPtr))
			{
				outScalarPtr->getMatrix().setDenseData(&inScalarValue, 1,1,MatrixData::DT_DOUBLE);
				outScalarPlug.setValue(outScalarObj);
			}
		}
	}
	else if (plug == m_outMatrix || plug.parent() == m_outMatrix)
	{
		MPlug outMatrixPlugArray = Global::getPlug(this, m_outMatrixName[0]);
		for (unsigned idxPhy = 0; idxPhy < outMatrixPlugArray.numElements(); ++idxPhy)
		{
			MPlug outMatrixPlug = outMatrixPlugArray.elementByPhysicalIndex(idxPhy, &s);
			CHECK_STATUS(s)
				if (!outMatrixPlug.isConnected())
					continue;
			CHECK_STATUS(s)
				int logicalIdx = outMatrixPlug.logicalIndex(&s);
			MPlug inMatrixPlugArray = Global::getPlug(this, m_inMatrixName[0]);
			MPlug inMatrixPlug = inMatrixPlugArray.elementByLogicalIndex(logicalIdx,&s);
			CHECK_STATUS(s)
				MObject inMatrixObject = inMatrixPlug.asMObject();
			MFnMatrixData inMatrixData(inMatrixObject);
			const MMatrix& inMatrix = inMatrixData.matrix();
			MObject outMatrixObj;
			MatlabMatrix* outMatrixPtr;
			double  matData[4][4];
			inMatrix.transpose().get(matData);
			if (MatlabMatrix::createMatrixObject(outMatrixObj, outMatrixPtr))
			{
				outMatrixPtr->getMatrix().setDenseData(matData[0], 4,4,MatrixData::DT_DOUBLE);
				outMatrixPlug.setValue(outMatrixObj);
			}
		}
	}
	else if (plug == m_outPoint || plug.parent() == m_outPoint)
	{
		MPlug outPointPlugArray = Global::getPlug(this, m_outPointName[0]);
		MPlug inPointPlugArray  = Global::getPlug(this, m_inPointName[0]);
		for (unsigned idxPhy = 0; idxPhy < outPointPlugArray.numElements(); ++idxPhy)
		{
			// get plugs
			MPlug outPointPlug = outPointPlugArray.elementByPhysicalIndex(idxPhy, &s);
			CHECK_STATUS(s)
			if (!outPointPlug.isConnected())
				continue;
			CHECK_STATUS(s)
			int logicalIdx = outPointPlug.logicalIndex(&s);
			MPlug inPointPlug = inPointPlugArray.elementByLogicalIndex(logicalIdx,&s);
			CHECK_STATUS(s)

			// get input data 
			MObject inPointObject = inPointPlug.asMObject();
			MFnNumericData inPointData(inPointObject);
			double pntData[3] = {0};
			inPointData.getData3Double(pntData[0], pntData[1], pntData[2]);

			// set output data
			MObject outPointObj;
			MatlabMatrix* outPointPtr;
			if (MatlabMatrix::createMatrixObject(outPointObj, outPointPtr))
			{
				outPointPtr->getMatrix().setDenseData(pntData, 1, 3, MatrixData::DT_DOUBLE);
				outPointPlug.setValue(outPointObj);
			}
		}
	}
	data.setClean(plug);
	return s;
}

MStatus DataToMatrixNode::initialize()
{
	MStatus s; 
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute  mAttr;

	m_inScalar = nAttr.create(m_inScalarName[0], m_inScalarName[1], MFnNumericData::kDouble,0);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setReadable(true); 
	nAttr.setSoftMin(0);
	nAttr.setSoftMax(1);
	nAttr.setArray(true);
	nAttr.setUsesArrayDataBuilder(true);

	m_outScalar = tAttr.create(m_outScalarName[0], m_outScalarName[1], MFnData::kPlugin);
	tAttr.setKeyable(false);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(false);
	tAttr.setReadable(true);
	tAttr.setArray(true);
	tAttr.setUsesArrayDataBuilder(true);


	m_inMatrix =  mAttr.create(m_inMatrixName[0], m_inMatrixName[1], MFnMatrixAttribute::kDouble);
	mAttr.setKeyable(true);
	mAttr.setStorable(true);
	mAttr.setHidden(false);
	mAttr.setWritable(true);
	mAttr.setReadable(true);
	mAttr.setArray(true);
	mAttr.setUsesArrayDataBuilder(true);


	m_outMatrix = tAttr.create(m_outMatrixName[0], m_outMatrixName[1], MFnData::kPlugin);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(false);
	tAttr.setArray(true);
	tAttr.setUsesArrayDataBuilder(true);

	m_inPoint = nAttr.create(m_inPointName[0], m_inPointName[1], MFnNumericData::k3Double, 0.0, &s);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setReadable(true);
	nAttr.setArray(true);
	nAttr.setUsesArrayDataBuilder(true);

	m_outPoint = tAttr.create(m_outPointName[0], m_outPointName[1], MFnData::kPlugin, &s);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(false);
	tAttr.setArray(true);
	tAttr.setUsesArrayDataBuilder(true);

	CHECK_STATUS(s);
	s = addAttribute( m_inScalar );
	CHECK_STATUS(s);
	s = addAttribute( m_outScalar ); 
	CHECK_STATUS(s);
	s = addAttribute( m_inMatrix );
	CHECK_STATUS(s);
	s = addAttribute( m_outMatrix);
	CHECK_STATUS(s);
	s = addAttribute( m_inPoint);
	CHECK_STATUS(s);
	s = addAttribute( m_outPoint);
	CHECK_STATUS(s);

	s = attributeAffects(m_inScalar, m_outScalar);
	CHECK_STATUS(s);
	s = attributeAffects(m_inMatrix, m_outMatrix);
	CHECK_STATUS(s);
	s = attributeAffects(m_inPoint, m_outPoint);
	CHECK_STATUS(s);

	return MS::kSuccess;
}



