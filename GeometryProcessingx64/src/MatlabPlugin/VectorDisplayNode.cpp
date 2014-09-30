#include "stdafx.h"
#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include "VectorDisplayNode.h"


MObject VectorDisplayNode::m_verticesCoord;
MObject VectorDisplayNode::m_vectorsPerVertex;
MObject VectorDisplayNode::m_transformMatrix;
MObject VectorDisplayNode::m_colorSetting;
MObject VectorDisplayNode::m_vectorLengthArray;
MObject VectorDisplayNode::m_currentIndex;

const char* VectorDisplayNode::m_nodeName = NODE_VECTOR_DISPLAY_NAME;
const MTypeId VectorDisplayNode::m_id(NODE_VECTOR_DISPLAY_ID);

const char* VectorDisplayNode::m_vectorsPerVertexString[2] = {"vectorsPerVertex","vecs"};
const char* VectorDisplayNode::m_verticesCoordString[2] = {"verticesCoord","vCoord"};
const char* VectorDisplayNode::m_transformMatrixString[2] = {"transformMatrix","transfMat"};
const char*	VectorDisplayNode::m_colorSettingString[2] = {"colorArray","colorArray"};
const char* VectorDisplayNode::m_vectorLengthArrayString[2] = {"vectorLengthArray","vecLenArray"};
const char*	VectorDisplayNode::m_currentIndexString[2] = {"currentIndex","curIdx"};

const float VectorDisplayNode::m_iconShape1[][2] = 
{
	0.f,	0.f,
	0.75f,	0.f,
	0.69f,	0.29f,
	0.53f,	0.53f,
	0.29f,	0.69f,
	0.f,	0.75f
};

const float VectorDisplayNode::m_iconShape2[][2] =
{
	0.69f,		0.29f,
	0.69f*1.2f,	0.29f*1.2f,
	0.53f,		0.53f,
	0.53f*1.2f,	0.53f*1.2f,
	0.29f,		0.69f,
	0.29f*1.2f,	0.69f*1.2f,

	0.31f,		0.54f,
	0.31f*1.6f,	0.54f*1.6f,
	0.54f,		0.31f,
	0.54f*1.6f,	0.31f*1.6f,

	0.28f,		0.28f,
	0.28f*1.7f,	0.28f*1.7f,
};
VectorDisplayNode::VectorDisplayNode():MPxLocatorNode(){
	m_box = getBox();	
}

MStatus VectorDisplayNode::initialize(){
	MStatus s;
	MFnTypedAttribute tAttr;
	MFnMatrixAttribute mAttr;
	MFnNumericAttribute nAttr;
	
	m_verticesCoord = tAttr.create(m_verticesCoordString[0],m_verticesCoordString[1],MFnData::kPlugin);
	tAttr.setKeyable(true);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(false);

	m_colorSetting = nAttr.createColor(m_colorSettingString[0],m_colorSettingString[1]);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setReadable(true); 
	nAttr.setArray(true);
	nAttr.setUsesArrayDataBuilder(true);
	nAttr.setUsedAsColor(true);


	m_vectorsPerVertex = tAttr.create(m_vectorsPerVertexString[0],m_vectorsPerVertexString[1],MFnData::kPlugin);
	tAttr.setKeyable(false);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(false);
	tAttr.setArray(true);
	tAttr.setUsesArrayDataBuilder(true);

	m_transformMatrix = mAttr.create(m_transformMatrixString[0],m_transformMatrixString[1]);
	mAttr.setHidden(false);
	mAttr.setReadable(false);
	mAttr.setWritable(true);
	mAttr.setKeyable(true);

	m_currentIndex = nAttr.create(m_currentIndexString[0],m_currentIndexString[1],MFnNumericData::kInt,0);
	tAttr.setKeyable(false);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(true);

	m_vectorLengthArray = nAttr.create(m_vectorLengthArrayString[0], m_vectorLengthArrayString[1], MFnNumericData::kDouble,0.5);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setReadable(true); 
	nAttr.setSoftMin(0);
	nAttr.setSoftMax(1);
	nAttr.setArray(true);
	nAttr.setUsesArrayDataBuilder(true);

	s = addAttribute(m_verticesCoord);
	CHECK_STATUS(s);
	s = addAttribute(m_vectorsPerVertex);
	CHECK_STATUS(s);
	s = addAttribute(m_transformMatrix);
	CHECK_STATUS(s);
	//s = addAttribute(m_redColorArray);
	//CHECK_STATUS(s);
	//s = addAttribute(m_greenColorArray);
	//CHECK_STATUS(s);
	//s = addAttribute(m_blueColorArray);
	//CHECK_STATUS(s);
	s = addAttribute(m_vectorLengthArray);
	CHECK_STATUS(s);
	s = addAttribute(m_colorSetting);
	CHECK_STATUS(s);
	s = addAttribute(m_currentIndex);
	CHECK_STATUS(s);


	return s;

}

MStatus VectorDisplayNode::compute( const MPlug& plug, MDataBlock& data ){
	return MS::kSuccess;
}

MBoundingBox VectorDisplayNode::boundingBox() const{
	return m_box;
}

void VectorDisplayNode::draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView:: DisplayStatus ){

	view.beginGL();
	glPushAttrib(GL_CURRENT_BIT);

	drawIcon();

	MMatrix m = path.inclusiveMatrixInverse();
	drawVectors(m);

	glPopAttrib();
	view.endGL();
}

MMatrix VectorDisplayNode::getTransformMatrix(){
	MStatus s;
	MPlug transfMatPlug = Global::getPlug(this,m_transformMatrixString[0]);
	if (!transfMatPlug.isConnected())
	{
		return MMatrix::identity;
	}
	MFnMatrixData objMatFn(transfMatPlug.asMObject());
	return objMatFn.matrix();
}

MStatus VectorDisplayNode::drawVectors(const MMatrix& invTransform){

	MStatus s;
	
	MPlug vertCoordPlug = Global::getPlug(this,m_verticesCoordString[0]);
	MPlug vecPerVertPlug = Global::getPlug(this,m_vectorsPerVertexString[0]);


	if (!vertCoordPlug.isConnected() )
	{
		return MS::kFailure;
	}

	
	MObject		valueArrayObj;
	
	//get vertex coordinate
	vertCoordPlug.getValue(valueArrayObj);
	MFnPluginData valueArrayFn(valueArrayObj,&s);
	CHECK_STATUS(s);
	if (valueArrayFn.typeId(&s) != MatlabMatrix::getTypeId())
	{
		MGlobal::displayError("Wrong data type. Need matlabMatrix type.");
		return MS::kFailure;
	}

	MatlabMatrix* pVertCoordMatrix = (MatlabMatrix*)valueArrayFn.data(&s);
	if (!pVertCoordMatrix)
	{
		MGlobal::displayError("vertices coordinate array is invalid.");
		return MS::kFailure;
	}

	MatrixData& vertMat = pVertCoordMatrix->getMatrix();
	size_t nVertRows = vertMat.getRows();
	size_t nVertCols = vertMat.getCols();
	if ( vertMat.getMatrixType() != MatrixData::MT_DENSE || nVertCols != 3 )
	{
		MGlobal::displayError("Wrong matrix dimension or matrix type. Need dense matrix with 3-dimension");
		return MS::kFailure;
	}
	
	
	
	//get raw data
	double* vertCoordData = new double[nVertRows*nVertCols];
	vertMat.getDenseDataDouble(vertCoordData);
	

	//get transform matrix
	MMatrix transfMat = getTransformMatrix() * invTransform;

	//loop draw
	glBegin(GL_POINTS);
	for (int idx = 0; idx < nVertRows; idx++)
	{
		MPoint vert(vertCoordData[idx],
			vertCoordData[idx+nVertRows],
			vertCoordData[idx+2*nVertRows]);

		vert    = vert    * transfMat;

		glVertex3d(vert.x,vert.y,vert.z);
	}
	glEnd();
	for (unsigned phyIdx = 0; phyIdx < vecPerVertPlug.numElements(&s); phyIdx++)
	{
		MPlug ithVecPlug = vecPerVertPlug.elementByPhysicalIndex(phyIdx,&s);
		int   logicalIdx = ithVecPlug.logicalIndex(&s);
		if (!s)
			continue;
		MColor color = getVectorColor(logicalIdx);
		double lenFactor = getVectorLengthFactor(logicalIdx);
		drawVectorPerVertex(vertCoordData,nVertRows,nVertCols,ithVecPlug,transfMat,color,lenFactor);
	}	

	delete[] vertCoordData;

	return s;



}

double VectorDisplayNode::getVectorLengthFactor(unsigned idx){
	
	MPlug vecLenArrayPlug = Global::getPlug(this,m_vectorLengthArrayString[0]);
	MPlug vecLenPlug = vecLenArrayPlug.elementByLogicalIndex(idx);

	return vecLenPlug.asDouble();
}

MColor VectorDisplayNode::getVectorColor(unsigned idx){

	MStatus s;

	MPlug colorPlug = Global::getPlug(this, m_colorSettingString[0]);
	MPlug currentColorPlug = colorPlug.elementByLogicalIndex(idx);
	return Global::getColor(currentColorPlug);
}

MStatus VectorDisplayNode::drawVectorPerVertex(double* vertCoordData,int nVertRows,int nVertCols,MPlug vecPerVertPlug,MMatrix& transfMat,MColor& color,double lenFactor){
		
	MStatus s;
	//get vectors array per vertex
	MObject		valueArrayObj;
	vecPerVertPlug.getValue(valueArrayObj);

	MatlabData* pMatlabData = NULL;
	s = MatlabData::checkAndGetData(valueArrayObj, pMatlabData);
	if (!s || !pMatlabData)
	{
		MGlobal::displayError("can't get matlab data");
		return MS::kFailure;
	}


	MatrixData& vecMat = (dynamic_cast<MatlabMatrix*>(pMatlabData))->getMatrix();
	size_t nVecRows = vecMat.getRows();
	size_t nVecCols = vecMat.getCols();
	if (vecMat.getMatrixType() != MatrixData::MT_DENSE || nVecCols != 3)
	{
		MGlobal::displayError("Wrong matrix dimension or matrix type. Need dense matrix with 3-dimension");
		return MS::kFailure;
	}



	if ( nVertRows != nVecRows)
	{
		MGlobal::displayError("inconsistency between number of vertices and number of vectors");
		return MS::kFailure;
	}

	double* vecPerVertData = new double[nVecRows*nVecCols];
	vecMat.getDenseDataDouble(vecPerVertData);
	
	//begin draw
	m_box = getBox();
	glBegin(GL_LINES);
	for (int idx = 0; idx < nVertRows; idx++)
	{
		MPoint vert(vertCoordData[idx],
			vertCoordData[idx+nVertRows],
			vertCoordData[idx+2*nVertRows]);

		MVector vec(vecPerVertData[idx],
			vecPerVertData[idx+nVertRows],
			vecPerVertData[idx+2*nVertRows]);

		//vec.normalize();
		vec *= lenFactor;
		MPoint vertEnd = vert + vec;

		vert    = vert    * transfMat;
		vertEnd = vertEnd * transfMat;

		glColor3d(color.r,color.g,color.b);
		glVertex3d(vert.x,vert.y,vert.z);
		glVertex3d(vertEnd.x, vertEnd.y, vertEnd.z);
		m_box.expand(vert);
		m_box.expand(vert+vec);
	}
	glEnd();

	delete[] vecPerVertData;
	return MS::kSuccess;
}

MBoundingBox VectorDisplayNode::getBox()
{
	MBoundingBox box;
	box.clear();
	box.expand(MPoint(1,0.1,1));
	box.expand(MPoint(-1,-0.1,-1));
	return box;
}

MStatus VectorDisplayNode::drawIcon()
{
	const int nIcon = 3;
	const float size = 0.5f;
	const float sqrt2 = 1.414f * 0.5f * size;

	glScalef(2.f,2.f,2.f);
	glBegin(GL_LINE_LOOP);
	const int s1Size = sizeof(m_iconShape1);
	const int s2Size = sizeof(m_iconShape2);
	for (int ithVtx=0; ithVtx < 6; ++ithVtx)
		glVertex3f(m_iconShape1[ithVtx][0], 0, m_iconShape1[ithVtx][1]);
	glEnd();

	glBegin(GL_LINES);
	for (int ithVtx=0; ithVtx < 6; ++ithVtx)
	{
		const int idx = ithVtx * 2;
		glVertex3f(m_iconShape2[idx][0], 0, m_iconShape2[idx][1]);
		glVertex3f(m_iconShape2[idx+1][0], 0, m_iconShape2[idx+1][1]);
	}
	glEnd();
	glScalef(0.5f,0.5f,0.5f);

	
	return MS::kSuccess;
}


