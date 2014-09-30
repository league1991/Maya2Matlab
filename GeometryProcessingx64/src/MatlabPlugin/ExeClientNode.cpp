#include "StdAfx.h"
#include "ExeClientNode.h"

MTypeId		ExeClientNode::m_id(NODE_EXE_CLIENT_ID);
const char* ExeClientNode::m_nodeName = NODE_EXE_CLIENT_NAME;


MObject ExeClientNode::m_outputMatrixArray;
MObject ExeClientNode::m_inputMatrixArray;
MObject ExeClientNode::m_outputIdentifierArray;
MObject ExeClientNode::m_inputIdentifierArray;

MObject ExeClientNode::m_exePath;
MObject ExeClientNode::m_immediatelyUpdate;
MObject ExeClientNode::m_resultString;
MObject ExeClientNode::m_maxComputeSeconds;
MObject ExeClientNode::m_maxTransferSeconds;
MObject ExeClientNode::m_taskIdentifier;

MObject ExeClientNode::m_ipAddress;
MObject ExeClientNode::m_port;




const char* ExeClientNode::m_inputMatrixArrayName[2]  = {"inputMatrix", "inMat"};
const char* ExeClientNode::m_outputMatrixArrayName[2] = {"outputMatrix", "outMat"};
const char* ExeClientNode::m_inputIdentifierArrayName[2]  = {"inputMatrixName", "inMatName"};
const char* ExeClientNode::m_outputIdentifierArrayName[2] = {"outputMatrixName", "outMatName"};


const char* ExeClientNode::m_exePathName[2]           = {"exePath","path"};
const char* ExeClientNode::m_taskIdentifierName[2]    = {"taskName", "tskName"};
const char* ExeClientNode::m_immediatelyUpdateName[2] = {"immediateUpdate","imUpdate"};
const char* ExeClientNode::m_resultStringName[2]      = {"result","res"};
const char* ExeClientNode::m_maxComputeSecondsName[2] = {"maxSecondsToCompute","maxSecToCmp"};
const char* ExeClientNode::m_maxTransferSecondsName[2]= {"maxSecondsToTransfer","maxSecToTrans"};

const char* ExeClientNode::m_ipAddressName[2]         = {"IPAddress","IPAdd"};
const char* ExeClientNode::m_portName[2]              = {"port","port"};

const float ExeClientNode::m_exeIconCurve[] = {
	// part1, frame 
	-0.1f,	0.0f,	0.0f,		
	-0.1f,	0.0f,	-1.0f,	
	-1.6f,	0.0f,	-1.0f,	
	-1.6f,	0.0f,	0.0f,	
	// part2f,	 title bar
	-0.1f,	0.0f,	-0.75f,	
	-1.6f,	0.0f,	-0.75f,	
	// part3f,	 close button
	-0.35f,	0.0f,	-0.75f,	
	-0.35f,	0.0f,	-1.0f,	
	// part4f,	 close cross(1)
	-0.3f,	0.0f,	-0.8f,	
	-0.15f,	0.0f,	-0.95f,	
	// part5f,	 close cross(2)
	-0.3f,	0.0f,	-0.95f,	
	-0.15f,	0.0f,	-0.8f
};

ExeClientNode::ExeClientNode(void)
{
	m_objectName = "*";
	buildNodeShape(m_objectName);
}

ExeClientNode::~ExeClientNode(void)
{
}

MStatus ExeClientNode::initialize()
{
	MStatus stat;

	// add instruction attribute
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;

	m_inputMatrixArray = tAttr.create(m_inputMatrixArrayName[0], m_inputMatrixArrayName[1], MFnData::kPlugin);
	{
		tAttr.setKeyable(false);
		tAttr.setStorable(true);
		tAttr.setHidden(false);
		tAttr.setWritable(true);
		tAttr.setReadable(false);
		tAttr.setArray(true);  
		tAttr.setUsesArrayDataBuilder(true);
	}

	m_outputMatrixArray = tAttr.create(m_outputMatrixArrayName[0], m_outputMatrixArrayName[1], MFnData::kPlugin);
	{
		tAttr.setKeyable(false);
		tAttr.setStorable(true);
		tAttr.setHidden(false);
		tAttr.setWritable(false);
		tAttr.setReadable(true);
		tAttr.setArray(true);
		tAttr.setUsesArrayDataBuilder(true);
	}
	m_inputIdentifierArray = tAttr.create(m_inputIdentifierArrayName[0], m_inputIdentifierArrayName[1], MFnData::kString);
	{
		tAttr.setKeyable(true);
		tAttr.setStorable(true);
		tAttr.setHidden(false);
		tAttr.setWritable(true);
		tAttr.setReadable(true);
		tAttr.setArray(true);
		tAttr.setUsesArrayDataBuilder(true);
	}

	m_outputIdentifierArray = tAttr.create(m_outputIdentifierArrayName[0], m_outputIdentifierArrayName[1], MFnData::kString);
	{
		tAttr.setKeyable(true);
		tAttr.setStorable(true);
		tAttr.setHidden(false);
		tAttr.setWritable(true);
		tAttr.setReadable(true);
		tAttr.setArray(true);
		tAttr.setUsesArrayDataBuilder(true); 
	}

	m_resultString = tAttr.create(m_resultStringName[0], m_resultStringName[1], MFnData::kString);
	{
		tAttr.setKeyable(false);
		tAttr.setStorable(true);
		tAttr.setHidden(true);
		tAttr.setWritable(true);
		tAttr.setReadable(false); 
	}	


	m_exePath = tAttr.create(m_exePathName[0], m_exePathName[1], MFnData::kString);
	{
		tAttr.setKeyable(false);
		tAttr.setStorable(true);
		tAttr.setHidden(true);
		tAttr.setWritable(true);
		tAttr.setReadable(false); 
		tAttr.setUsedAsFilename(true);
	}	
	m_immediatelyUpdate = nAttr.create(m_immediatelyUpdateName[0], m_immediatelyUpdateName[1], MFnNumericData::kBoolean, false);
	{
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setHidden(false);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
	}

	m_maxComputeSeconds = nAttr.create(m_maxComputeSecondsName[0], m_maxComputeSecondsName[1], MFnNumericData::kInt, 3);
	{
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setHidden(false);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		nAttr.setMin(1);
		nAttr.setSoftMin(1);
		nAttr.setSoftMax(20);
	}
	m_maxTransferSeconds = nAttr.create(m_maxTransferSecondsName[0], m_maxTransferSecondsName[1], MFnNumericData::kInt, 3);
	{
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setHidden(false);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		nAttr.setMin(1);
		nAttr.setSoftMin(1);
		nAttr.setSoftMax(20);
	}

	m_taskIdentifier = tAttr.create(m_taskIdentifierName[0], m_taskIdentifierName[1], MFnData::kString);
	{
		tAttr.setKeyable(true);
		tAttr.setStorable(true);
		tAttr.setHidden(false);
		tAttr.setWritable(true);
		tAttr.setReadable(true);
	}

	m_ipAddress = tAttr.create(m_ipAddressName[0], m_ipAddressName[1], MFnData::kString);
	{
		tAttr.setKeyable(true);
		tAttr.setStorable(true);
		tAttr.setHidden(false);
		tAttr.setWritable(true);
		tAttr.setReadable(true);
	}
	m_port = nAttr.create(m_portName[0], m_portName[1], MFnNumericData::kInt, 1234);
	{
		nAttr.setKeyable(true);
		nAttr.setStorable(true);
		nAttr.setHidden(false);
		nAttr.setWritable(true);
		nAttr.setReadable(true);
		nAttr.setMin(1);
		nAttr.setMax(65535);
	}


	// Add the attributes we have created to the node
	//
	CHECK_STATUS(stat);
	stat = addAttribute( m_inputMatrixArray );
	CHECK_STATUS(stat);
	stat = addAttribute( m_outputMatrixArray );
	CHECK_STATUS(stat);
	stat = addAttribute( m_inputIdentifierArray );
	CHECK_STATUS(stat);
	stat = addAttribute( m_outputIdentifierArray );

	CHECK_STATUS(stat);
	stat = addAttribute( m_exePath );
	CHECK_STATUS(stat);
	stat = addAttribute( m_resultString );
	CHECK_STATUS(stat);
	stat = addAttribute( m_immediatelyUpdate );
	CHECK_STATUS(stat);
	stat = addAttribute( m_maxComputeSeconds );
	CHECK_STATUS(stat);
	stat = addAttribute( m_maxTransferSeconds);
	CHECK_STATUS(stat);
	stat = addAttribute( m_taskIdentifier );
	CHECK_STATUS(stat);
	stat = addAttribute( m_ipAddress );
	CHECK_STATUS(stat);
	stat = addAttribute( m_port );
	CHECK_STATUS(stat);

	// Set up a dependency between the input and the output.  This will cause
	// the output to be marked dirty when the input changes.  The output will
	// then be recomputed the next time the value of the output is requested.
	//
	stat = attributeAffects( m_inputMatrixArray, m_outputMatrixArray );
	if (!stat) { stat.perror("attributeAffects fail."); return stat;}
	stat = attributeAffects( m_immediatelyUpdate, m_outputMatrixArray );
	if (!stat) { stat.perror("attributeAffects fail."); return stat;}

	return MS::kSuccess;
}

void ExeClientNode::draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView:: DisplayStatus )
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

MBoundingBox ExeClientNode::boundingBox() const
{
	return m_box;
}

void ExeClientNode::drawIcon()
{
	glBegin(GL_LINE_LOOP);
	glVertex3fv(&m_exeIconCurve[0]);
	glVertex3fv(&m_exeIconCurve[3]);
	glVertex3fv(&m_exeIconCurve[6]);
	glVertex3fv(&m_exeIconCurve[9]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3fv(&m_exeIconCurve[12]);
	glVertex3fv(&m_exeIconCurve[15]);

	glVertex3fv(&m_exeIconCurve[18]);
	glVertex3fv(&m_exeIconCurve[21]);

	glVertex3fv(&m_exeIconCurve[24]);
	glVertex3fv(&m_exeIconCurve[27]);

	glVertex3fv(&m_exeIconCurve[30]);
	glVertex3fv(&m_exeIconCurve[33]);
	glEnd();
}
void ExeClientNode::drawText()
{
	QPolygonF& poly = m_textShape;
	if (poly.size() != 0)
	{
		QPointF p0 = poly[0];
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


void ExeClientNode::buildNodeShape( const MString& name )
{
	QString qName = MQtUtil::toQString(name);
	QPainterPath path;
	QFont font("Arial");
	path.addText(0,0, font, qName); 
	//path.closeSubpath();
	//m_textShapes = path.toFillPolygons();
	m_textShape = path.toFillPolygon();

	float boxMin[2] = {-1.6f,-1.f};
	float boxMax[2] = {-0.f,-0.f};
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

MStatus ExeClientNode::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus s;

	MPlug imUpdatePlug = Global::getPlug(this, m_immediatelyUpdateName[0]);
	bool  isImUpdate = false;
	imUpdatePlug.getValue(isImUpdate);
	if (isImUpdate)
	{
		s = computeExe();
	}
	data.setClean(plug);
	return MS::kSuccess;
}

MStatus ExeClientNode::computeExe()
{
	MStatus s;
	MString ipMStr;
	int port;

	Global::getPlug(this, m_ipAddressName[0]).getValue(ipMStr);
	QString ipQStr = MQtUtil::toQString(ipMStr);
	CHECK_EXPRESSION(ipQStr.trimmed() == "", "invalid IP address.");
	Global::getPlug(this, m_portName[0]).getValue(port);

	// get plugs
	MPlug inputIDArrayPlug = Global::getPlug(this, m_inputIdentifierArrayName[0]);
	MPlug inputArrayPlug   = Global::getPlug(this, m_inputMatrixArrayName[0]);
	MPlug outputArrayPlug = Global::getPlug(this, m_outputMatrixArrayName[0]);
	MString taskNameMStr;
	QString taskNameQStr;
	QHash<QString,int> outputMap;
	getOutputIDMap(outputMap);

	ExeClient client;
#define CHECK_AND_GOTO(x,str)if(x){MGlobal::displayError(str); goto SESSION_END;}
	CHECK_AND_GOTO(!client.connectToServer(ipQStr, port), "failed to connect to EXE server");

	// begin task
	int maxTransferSeconds, maxComputeSeconds;
	Global::getPlug(this, m_maxTransferSecondsName[0]).getValue(maxTransferSeconds);
	Global::getPlug(this, m_maxComputeSecondsName[0]).getValue(maxComputeSeconds);
	Global::getPlug(this, m_taskIdentifierName[0]).getValue(taskNameMStr);
	taskNameQStr = MQtUtil::toQString(taskNameMStr);
	CHECK_AND_GOTO(!client.taskBegin(taskNameQStr, numInputs(), maxTransferSeconds, maxComputeSeconds), "failed to begin task");

	// send input
	int nInputs = inputIDArrayPlug.numElements();
	for (unsigned ithPlug = 0; ithPlug < (unsigned)nInputs; ++ithPlug)
	{
		MPlug inputIDPlug = inputIDArrayPlug.elementByPhysicalIndex(ithPlug, &s);
		CHECK_AND_GOTO(!s, "failed to get input id array plug.");

		MString inputIDMStr;
		inputIDPlug.getValue(inputIDMStr);
		QString inputIDQStr = MQtUtil::toQString(inputIDMStr);
		if (inputIDQStr.trimmed() == "")
			continue;

		unsigned logicalIdx = inputIDPlug.logicalIndex(&s);
		CHECK_AND_GOTO(!s, "failed to get logical index.");
		MPlug   inputMatPlug = inputArrayPlug.elementByLogicalIndex(logicalIdx, &s);
		CHECK_AND_GOTO(!s, "failed to get input matrix plug.");

		MObject inputMatObj;
		MatlabMatrix* pMat;
		inputMatPlug.getValue(inputMatObj);
		s = MatlabMatrix::checkAndGetMatrixPtr(inputMatObj, pMat);

		CHECK_AND_GOTO(!client.sendInput(inputIDQStr, &pMat->getMatrix()), "error occurs when sending data.");
	}

	// compute
	CHECK_AND_GOTO(!client.compute(), "computation error.");

	for (int ithOut = 0; ithOut < outputMap.size(); ++ithOut)
	{
		MObject outObj;
		MatlabMatrix* outPtr;
		s = MatlabMatrix::createMatrixObject(outObj, outPtr);
		CHECK_AND_GOTO(!s, "failed to create matlab matrix object.");

		QString outName;
		CHECK_AND_GOTO(!client.receiveOutput(outName, &outPtr->getMatrix()), "failed receive output");
		if (!outputMap.contains(outName))
			continue;

		int logicalIdx = outputMap[outName];
		outputArrayPlug.elementByLogicalIndex(logicalIdx).setValue(outObj);
	}

	CHECK_AND_GOTO(!client.taskEnd(), "failed to end task.");

SESSION_END:
	Global::getPlug(this, m_resultStringName[0]).setValue(MQtUtil::toMString(client.getInfo()));
	return s;
}

int ExeClientNode::numInputs()
{
	int nValidInputs = 0;
	MPlug inputIDArrayPlug = Global::getPlug(this, m_inputIdentifierArrayName[0]);
	for (unsigned ithPlug = 0; ithPlug < inputIDArrayPlug.numElements(); ++ithPlug)
	{
		MPlug inputIDPlug = inputIDArrayPlug.elementByPhysicalIndex(ithPlug);

		MString inputIDMStr;
		inputIDPlug.getValue(inputIDMStr);
		if (inputIDMStr != "")
			nValidInputs++;
	}
	return nValidInputs;
}

MStatus ExeClientNode::getOutputIDMap( QHash<QString,int>& outputMap )
{
	MStatus s;
	MPlug outputIDArrayPlug = Global::getPlug(this, m_outputIdentifierArrayName[0]);
	for (unsigned ithPlug = 0; ithPlug < outputIDArrayPlug.numElements(); ++ithPlug)
	{
		MPlug outputIDPlug = outputIDArrayPlug.elementByPhysicalIndex(ithPlug, &s);
		CHECK_STATUS(s);

		MString outputIDMStr;
		outputIDPlug.getValue(outputIDMStr);
		if (outputIDMStr == "")
			continue;

		int logicalIdx = outputIDPlug.logicalIndex(&s);
		CHECK_STATUS(s);
		outputMap[MQtUtil::toQString(outputIDMStr)] = logicalIdx;
	}
	return s;
}

