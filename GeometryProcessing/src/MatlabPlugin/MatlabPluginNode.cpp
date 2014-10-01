//
// Copyright (C) 
// 
// File: MatlabPluginNode.cpp
//
// Dependency Graph Node: MatlabPlugin
//
// Author: Maya Plug-in Wizard 2.0
//
#include "stdafx.h"
#include "MatlabPluginNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MGlobal.h>





Engine* MatlabEngine::m_engine = NULL;


// You MUST change this to a unique value!!!  The id is a 32bit value used
// to identify this type of node in the binary file format.  
//
// #error change the following to a unique value and then erase this line 
MTypeId     MatlabNode::m_id( NODE_MATLAB_ID );

const char* MatlabNode::m_nodeName = NODE_MATLAB_NAME;

MObject		MatlabNode::m_instString;
MObject     MatlabNode::m_curDirString;
MObject		MatlabNode::m_resultString;
MObject		MatlabNode::m_inputMatrixArray;
MObject     MatlabNode::m_outputMatrixArray;
MObject		MatlabNode::m_immediateUpdate;

const float MatlabNode::m_matlabIconCurve[] = {
	// part 1
	-0.9001f,	0,	-1.418f,			
	-0.8500f,	0,	-1.442f,	
	-0.7451f,	0,	-1.392f,	
	-0.6187f,	0,	-1.123f,	
	-0.2152f,	0,	-0.0075f,	
	-0.444f,	0,	-0.2314f,	
	-0.588f,	0,	-0.2831f,	
	-0.7262f,	0,	-0.2314f,	
	-0.9673f,	0,	0.04411f,	
	-1.179f,	0,	0.233f,	
	-1.288f,	0,	0.2852f,	
	-1.394f,	0,	-0.01610f,	
	-1.478f,	0,	-0.1617f,	
	-1.588f,	0,	-0.2394f,	
	-1.680f,	0,	-0.2096f,	
	-2.098f,	0,	-0.4955f,	
	-1.557f,	0,	-0.7145f,	 
	-1.392f,	0,	-0.9031f,	
	-1.300f,	0,	-0.9318f,	
	-1.162f,	0,	-1.035f,	
	-1.001f,	0,	-1.264f,	
	-0.9151f,	0,	-1.411f,	
	-1.231f,	0,	-0.6844f,	
	-1.588f,	0,	 -0.240f,	
	// part 2
	-1.550f,	0,	-0.7152f,	
	-1.331f,	0,	-0.5423f
};

const char* MatlabNode::m_instStringName[2]  = {"instructionString", "instStr"};
const char* MatlabNode::m_curDirStringName[2]= {"currentDirectory", "curDir"};
const char* MatlabNode::m_resultStringName[2]= {"result","res"};
const char* MatlabNode::m_inputMatrixArrayName[2]= {"inputMatrix", "inMat"};
const char* MatlabNode::m_outputMatrixArrayName[2]= {"outputMatrix", "outMat"};
const char* MatlabNode::m_immediateUpdateName[2]={"immediateUpdate","imUpdate"};

const int   MatlabNode::m_resultBufferLength = 1024 * 100;


Engine* MatlabEngine::getOrOpenEngine()
{
	if (!m_engine)
		m_engine = engOpen(NULL);
	return m_engine;
}

bool MatlabEngine::isEngineOpen()
{
	return m_engine != NULL;
}

bool MatlabEngine::openEngine()
{
	if (!m_engine)
		m_engine = engOpen(NULL);
	return m_engine != NULL;
}
bool MatlabEngine::closeEngine()
{
	if (m_engine)
	{
		engClose(m_engine);
		m_engine = NULL;
	}
	return true;
}

MatlabNode::MatlabNode() 
{
	m_resultBuffer = new char[m_resultBufferLength+2];
	m_resultBuffer[m_resultBufferLength]   = 0;
	m_resultBuffer[m_resultBufferLength+1] = 0;
	m_objectName = "*";
	buildNodeShape(m_objectName);
}
MatlabNode::~MatlabNode() 
{
	delete[] m_resultBuffer;
}

MStatus MatlabNode::compute( const MPlug& plug, MDataBlock& data )
//
//	Description:
//		This method computes the value of the given output plug based
//		on the values of the input attributes.
//
//	Arguments:
//		plug - the plug to compute
//		data - object that provides access to the attributes for this node
//
{
	MStatus s;
 
	// Check which output attribute we have been asked to compute.  If this 
	// node doesn't know how to compute it, we must return 
	// MS::kUnknownParameter.
	// 

	MPlug imUpdatePlug = Global::getPlug(this, m_immediateUpdateName[0]);
	bool  isImUpdate = false;
	imUpdatePlug.getValue(isImUpdate);
	if (isImUpdate)
	{
		Engine* eng = MatlabEngine::getOrOpenEngine();
		s = computeMatlab(eng);
	}
	data.setClean(plug);
	return MS::kSuccess;
}

void* MatlabNode::creator()
//
//	Description:
//		this method exists to give Maya a way to create new objects
//      of this type. 
//
//	Return Value:
//		a new object of this type
//
{
	return new MatlabNode();
}

MStatus MatlabNode::initialize()
//
//	Description:
//		This method is called to create and initialize all of the attributes
//      and attribute dependencies for this node type.  This is only called 
//		once when the node type is registered with Maya.
//
//	Return Values:
//		MS::kSuccess
//		MS::kFailure
//		
{
	// This sample creates a single input float attribute and a single
	// output float attribute.
	//
	MStatus				stat;

	// add instruction attribute
	MFnTypedAttribute   tAttr;
	MFnNumericAttribute nAttr;

	m_immediateUpdate = nAttr.create(m_immediateUpdateName[0], m_immediateUpdateName[1], MFnNumericData::kBoolean, false);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setReadable(true);

	m_instString = tAttr.create(m_instStringName[0], m_instStringName[1], MFnData::kString);
	tAttr.setKeyable(true);
	tAttr.setStorable(true);
	tAttr.setHidden(true);
	tAttr.setWritable(true);
	tAttr.setReadable(true);
	tAttr.setArray(false);

	m_curDirString = tAttr.create(m_curDirStringName[0], m_curDirStringName[1], MFnData::kString);
	tAttr.setKeyable(true);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);

	m_resultString = tAttr.create(m_resultStringName[0], m_resultStringName[1], MFnData::kString);
	tAttr.setKeyable(true);
	tAttr.setStorable(true);
	tAttr.setHidden(true);
	tAttr.setWritable(true);

	m_inputMatrixArray = tAttr.create(m_inputMatrixArrayName[0], m_inputMatrixArrayName[1], MFnData::kPlugin);
	tAttr.setKeyable(false);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(false);
	tAttr.setArray(true);  
	tAttr.setUsesArrayDataBuilder(true);
	tAttr.setAffectsAppearance(true);

	m_outputMatrixArray = tAttr.create(m_outputMatrixArrayName[0], m_outputMatrixArrayName[1], MFnData::kPlugin);
	tAttr.setKeyable(false);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(false);
	tAttr.setReadable(true);
	tAttr.setArray(true);
	tAttr.setUsesArrayDataBuilder(true);
	tAttr.setAffectsAppearance(true);

	// Add the attributes we have created to the node
	//
	CHECK_STATUS(stat);
	stat = addAttribute( m_immediateUpdate );
	CHECK_STATUS(stat);
	stat = addAttribute( m_instString );
	CHECK_STATUS(stat);
	stat = addAttribute( m_curDirString );
	CHECK_STATUS(stat);
	stat = addAttribute( m_resultString );
	CHECK_STATUS(stat);
	stat = addAttribute( m_inputMatrixArray );
	CHECK_STATUS(stat);
	stat = addAttribute( m_outputMatrixArray );
	CHECK_STATUS(stat);


	// Set up a dependency between the input and the output.  This will cause
	// the output to be marked dirty when the input changes.  The output will
	// then be recomputed the next time the value of the output is requested.
	//
	stat = attributeAffects( m_inputMatrixArray, m_outputMatrixArray );
	if (!stat) { stat.perror("attributeAffects fail."); return stat;}
	stat = attributeAffects( m_immediateUpdate, m_outputMatrixArray );
	if (!stat) { stat.perror("attributeAffects fail."); return stat;}
	stat = attributeAffects( m_instString, m_outputMatrixArray );
	if (!stat) { stat.perror("attributeAffects fail."); return stat;}

	return MS::kSuccess;

}

MStatus MatlabNode::computeMatlab(Engine* eng)
{
	//MString msg = "Computation Begin in " + this->name() + ".";
	//MGlobal::displayInfo(msg);

	// set buffer to display result
	engOutputBuffer(eng, m_resultBuffer, m_resultBufferLength);

	// find node plugs
	MStatus s;
	MObject nodeObj = thisMObject();
	MFnDependencyNode nodeFn(nodeObj, &s);
	MPlug instPlug = nodeFn.findPlug(m_instStringName[0], &s);
	CHECK_STATUS(s);
	MPlug curDirPlug = nodeFn.findPlug(m_curDirStringName[0], &s);
	CHECK_STATUS(s);
	MPlug resultPlug = nodeFn.findPlug(m_resultStringName[0], &s);
	CHECK_STATUS(s);
	MPlug inputPlug  = nodeFn.findPlug(m_inputMatrixArrayName[0], &s);
	CHECK_STATUS(s);
	MPlug outputPlug = nodeFn.findPlug(m_outputMatrixArrayName[0], &s);
	CHECK_STATUS(s);

	// set current directory
	int instLength;
	MString curDirStr= curDirPlug.asString();
	int res;
	if (curDirStr.numChars() != 0)
	{
		MString changeDirInst = "cd('" + curDirStr + "');";							
		res = engEvalString(eng, changeDirInst.asChar(instLength));
	}

	// send input data
	if(!sendInputsToMatlab(eng, inputPlug, &s))
	{
		MGlobal::displayError(this->name() + MString(": Failed to send data to matlab."));
		return s;
	}

	// exec command
	MString instStr  = instPlug.asString();
	res = engEvalString(eng, instStr.asChar(instLength));						// execute command

	// set result
	MString resStr(m_resultBuffer, m_resultBufferLength);
	resultPlug.setString(resStr);
	CHECK_STATUS(s);

	// receive output data
	if (!receiveOutputFromMatlab(eng, outputPlug, &s))
	{
		MGlobal::displayError(this->name() + MString(": Failed to get data from Matlab."));
		return s;
	}
	MString msg = "Computation Finished in " + this->name() + ".";
	MGlobal::displayInfo(msg);
	return s;
}

bool MatlabNode::sendInputsToMatlab( Engine* eng, MPlug& inputPlug, MStatus* s)
{
	MStatus status;
	if (s == NULL)
		s = &status;

	// prefetch, because getValue() may cause computation
	QHash<int, MObject> objMap;
	for (unsigned i = 0; i < inputPlug.numElements(s); i++)
	{
		MPlug ithElementPlug = inputPlug.elementByPhysicalIndex(i, s);
		if (!*s)
			continue;

		int logicalIdx     = ithElementPlug.logicalIndex(s);
		MObject& valObj    = objMap[logicalIdx];
		ithElementPlug.getValue(valObj);
	}

	// when all computation is finished, get value and send to matlab
	for (QHash<int, MObject>::Iterator pObj = objMap.begin(); pObj != objMap.end(); ++pObj)
	{
		int logicalIdx      = pObj.key();
		MObject& valObj     = pObj.value();
		MatlabData* pMat = NULL;
		*s = MatlabData::checkAndGetData(valObj, pMat);
		if (!*s || !pMat)
		{
			MGlobal::displayError(this->name() + MString(": can't get matlab data"));
			continue;
		}
		if (!pMat->sendDataToMatlab(eng, getInputArrayName(logicalIdx)))
		{
			return false;
		}
	}
	return true;
}

bool MatlabNode::receiveOutputFromMatlab( Engine* eng, MPlug& outputPlug, MStatus* s/*=NULL*/ )
{
	MStatus status;
	if (s == NULL)
		s = &status;
	for (unsigned i = 0; i < outputPlug.numElements(s); i++)
	{
		MPlug ithElementPlug = outputPlug.elementByPhysicalIndex(i, s);
		// only plugs with connection(s) will be considered
		//if (!ithElementPlug.isConnected(s))
		//	continue;

		// prepare data
		int logicalIdx     = ithElementPlug.logicalIndex(s);
		MString outName = getOutputArrayName(logicalIdx);
		int     outNameLength = 0;
		mxArray* matlabArray = engGetVariable(eng, outName.asChar(outNameLength));
		if (!matlabArray)
			return false;
		MatlabData::Type type = MatlabData::getDataType(matlabArray);
		MFnPluginData valFn;
		MObject       valObj;
		switch (type)
		{
		case MatlabData::MATLAB_STRUCT:
			valObj = valFn.create(MatlabStruct::getTypeId(), s); break;
		case MatlabData::MATLAB_MATRIX:
			valObj = valFn.create(MatlabMatrix::getTypeId(), s); break;
		}
		if (s->statusCode() != MStatus::kSuccess)
			return false;

		// extract plug data
		MatlabData* pMat = (MatlabData*)valFn.data(s);
		if (s->statusCode() != MStatus::kSuccess)
			return false;

		// get data from matlab
		if (pMat->getDataFromMatlab(matlabArray))
			ithElementPlug.setValue(valObj);

		mxDestroyArray(matlabArray);
	}
	return true;
}

MString MatlabNode::getOutputArrayName( int idx )
{
	char varNameBuf[20]={0};
	sprintf_s(varNameBuf, NODE_OUT_PLUG_FORMAT,idx);
	return  MString(varNameBuf);
}

MString MatlabNode::getInputArrayName( int idx )
{
	char varNameBuf[20]={0};
	sprintf_s(varNameBuf, NODE_IN_PLUG_FORMAT,idx);
	return MString(varNameBuf);
}

void MatlabNode::draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView:: DisplayStatus )
{
	MString newName = this->name();
	if (newName != m_objectName)
	{
		buildNodeShape(newName);
		m_objectName = newName;
	}
 
	view.beginGL();
	glPushAttrib(GL_CURRENT_BIT);

	drawText();
	drawIcon();
 
	glPopAttrib();
	view.endGL();
}

MBoundingBox MatlabNode::boundingBox() const
{ 
	return m_box;
}

void MatlabNode::buildNodeShape( const MString& name )
{
	QString qName = MQtUtil::toQString(name);
	QPainterPath path;
	QFont font("Arial");
	path.addText(0,0, font, qName); 
	//path.closeSubpath();
	//m_textShapes = path.toFillPolygons();
	m_textShape = path.toFillPolygon();
		
	float boxMin[2] = {-2.098f,-1.443f};
	float boxMax[2] = {-0.215f,-0.008f};
	QPolygonF& poly = m_textShape;//[ithPoly];
	for (int ithPoint = 0; ithPoint < poly.size(); ++ithPoint)
	{ 
		QPointF& p = poly[ithPoint];
		p *= 0.05;
		boxMin[0] = boxMin[0] < p.x() ? boxMin[0] : p.x();
		boxMin[1] = boxMin[1] < p.y() ? boxMin[1] : p.y();
		boxMax[0] = boxMax[0] > p.x() ? boxMax[0] : p.x();
		boxMax[1] = boxMax[1] > p.y() ? boxMax[1] : p.y();
	}
	m_box = MBoundingBox(MPoint(boxMin[0],-0.5, boxMin[1]),
		                 MPoint(boxMax[0], 0.5, boxMax[1]));

}

void MatlabNode::drawIcon()
{
	const int part1Length = sizeof(m_matlabIconCurve) / sizeof(float) / 3 - 2;
	glBegin(GL_LINE_STRIP);
	for (int idx = 0; idx < part1Length*3; idx+=3)
		glVertex3f(m_matlabIconCurve[idx],	m_matlabIconCurve[idx+1],m_matlabIconCurve[idx+2]);
	glEnd();

	const int idx = part1Length*3;
	glBegin(GL_LINES);
	glVertex3f(m_matlabIconCurve[idx],	m_matlabIconCurve[idx+1],m_matlabIconCurve[idx+2]);
	glVertex3f(m_matlabIconCurve[idx+3],	m_matlabIconCurve[idx+4],m_matlabIconCurve[idx+5]);
	glEnd();
}

void MatlabNode::drawText()
{
	QPolygonF& poly = m_textShape;
	if (poly.size() != 0)
	{
		QPointF p0 = poly[0];
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_LINE_LOOP);
		for (int ithPoint = 0; ithPoint < poly.size(); ++ithPoint)
		{
			QPointF& p = poly[ithPoint];
			if(p!= p0 || ithPoint == 0)
				glVertex3f(p.x(), 0, p.y());
			else
			{
				glEnd();
				glBegin(GL_LINE_LOOP);
				ithPoint++;
			}
		}
		glEnd();
	} 
}



