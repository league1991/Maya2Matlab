#include "StdAfx.h"
#include "ModelPainterNode.h"


const char* MeshModifierNode::m_nodeName = NODE_MODEL_PAINTER_NAME;
MTypeId MeshModifierNode::m_id(NODE_MODEL_PAINTER_ID);

MObject MeshModifierNode::m_maxValue;
MObject MeshModifierNode::m_minValue;
MObject MeshModifierNode::m_autoRange;
MObject MeshModifierNode::m_outMesh;
MObject MeshModifierNode::m_inMesh;
MObject MeshModifierNode::m_vertValArray;
MObject MeshModifierNode::m_norDirArray;
MObject MeshModifierNode::m_vertPosArray;
MObject MeshModifierNode::m_vertColorArray;
MObject MeshModifierNode::m_vertColorSource;

const char* MeshModifierNode::m_vertColorSourceName[2]={"vertexColorSource", "vClrSrc"};
const char* MeshModifierNode::m_vertColorArrayName[2]={"vertexColor", "vClr"};
const char* MeshModifierNode::m_vertValArrayName[2]={"vertexValue", "vVal"};
const char* MeshModifierNode::m_norDirArrayName[2]={"vertexNormal", "vNor"};
const char* MeshModifierNode::m_vertPosArrayName[2]={"vertexPosition","vPos"};
const char* MeshModifierNode::m_maxValueName[2]={"maxValue", "vMax"};
const char* MeshModifierNode::m_minValueName[2]={"minValue", "vMin"};
const char* MeshModifierNode::m_autoRangeName[2]={"autoRange","auto"};
const char* MeshModifierNode::m_outMeshName[2]={"outMesh","outMesh"};
const char* MeshModifierNode::m_inMeshName[2]={"inMesh","inMesh"};

const unsigned char  MeshModifierNode::m_colorMap0[300] = 
{
	12 , 60 , 83
	, 12 , 62 , 83
	, 10 , 65 , 84
	,9 , 68 , 85
	,7 , 71 , 86
	,5 , 74 , 86
	,3 , 78 , 86
	,1 , 82 , 87
	,0 , 87 , 88
	,0 , 91 , 89
	,0 , 95 , 89
	,0 ,100 , 90
	,0 ,104 , 91
	,0 ,108 , 91
	,0 ,114 , 93
	,0 ,119 , 94
	,0 ,123 , 94
	,0 ,127 , 95
	,0 ,132 , 96
	,0 ,136 , 97
	,0 ,140 , 98
	,0 ,145 , 99
	,0 ,149 ,100
	,0 ,152 ,100
	,6 ,156 ,101
	, 11 ,160 ,102
	, 17 ,164 ,103
	, 24 ,168 ,105
	, 33 ,173 ,107
	, 42 ,177 ,108
	, 51 ,181 ,110
	, 60 ,186 ,112
	, 70 ,190 ,113
	, 81 ,194 ,116
	, 91 ,200 ,117
	,102 ,204 ,119
	,113 ,208 ,121
	,124 ,212 ,122
	,135 ,216 ,124
	,146 ,220 ,125
	,157 ,223 ,125
	,168 ,226 ,124
	,177 ,230 ,125
	,187 ,232 ,125
	,197 ,234 ,125
	,205 ,235 ,125
	,213 ,236 ,124
	,221 ,237 ,124
	,227 ,237 ,125
	,233 ,237 ,125
	,238 ,237 ,124
	,244 ,234 ,121
	,248 ,232 ,118
	,251 ,229 ,114
	,255 ,225 ,110
	,255 ,220 ,105
	,255 ,215 , 99
	,255 ,210 , 93
	,255 ,203 , 87
	,255 ,197 , 81
	,255 ,189 , 75
	,255 ,182 , 68
	,255 ,175 , 61
	,255 ,168 , 55
	,255 ,160 , 48
	,255 ,153 , 41
	,255 ,146 , 36
	,255 ,138 , 30
	,255 ,131 , 24
	,255 ,125 , 18
	,255 ,118 , 14
	,255 ,112 , 10
	,255 ,107  ,6
	,255 ,101  ,2
	,255 , 97  ,1
	,253 , 93  ,0
	,251 , 89  ,0
	,247 , 86  ,0
	,243 , 83  ,0
	,238 , 79  ,0
	,234 , 77  ,0
	,229 , 73  ,0
	,224 , 71  ,0
	,218 , 67  ,1
	,212 , 66  ,2
	,206 , 63  ,4
	,200 , 60  ,5
	,194 , 58  ,7
	,188 , 56  ,9
	,182 , 54 , 11
	,176 , 52 , 13
	,170 , 49 , 16
	,164 , 48 , 18
	,158 , 46 , 21
	,152 , 44 , 23
	,148 , 43 , 25
	,143 , 42 , 26
	,138 , 40 , 28
	,133 , 38 , 30
	,130 , 37 , 30
};
const unsigned char MeshModifierNode::m_colorMap1[300] ={
	3,  86, 255
	,   5,  88, 255
	,   9,  90, 255
	,  12,  93, 255
	,  16,  95, 255
	,  20,  98, 255
	,  25, 101, 255
	,  29, 105, 255
	,  34, 108, 255
	,  38, 112, 255
	,  43, 116, 255
	,  49, 119, 255
	,  55, 124, 255
	,  60, 128, 255
	,  65, 132, 255
	,  71, 137, 255
	,  77, 141, 255
	,  83, 145, 255
	,  89, 149, 255
	,  95, 154, 255
	, 101, 159, 255
	, 107, 163, 255
	, 114, 168, 255
	, 121, 172, 255
	, 127, 178, 255
	, 133, 182, 255
	, 139, 187, 255
	, 146, 191, 255
	, 152, 195, 255
	, 158, 200, 255
	, 165, 204, 255
	, 171, 209, 255
	, 177, 213, 255
	, 182, 216, 255
	, 188, 220, 255
	, 194, 224, 255
	, 199, 228, 255
	, 205, 231, 255
	, 210, 234, 255
	, 215, 237, 255
	, 221, 240, 255
	, 225, 243, 255
	, 229, 245, 255
	, 234, 247, 255
	, 238, 249, 255
	, 242, 251, 255
	, 245, 252, 255
	, 248, 253, 255
	, 251, 254, 255
	, 255, 255, 255
	, 255, 255, 253
	, 255, 255, 250
	, 255, 255, 247
	, 255, 254, 243
	, 255, 253, 239
	, 255, 252, 235
	, 255, 251, 230
	, 255, 249, 225
	, 255, 247, 220
	, 255, 245, 215
	, 255, 242, 209
	, 255, 240, 203
	, 255, 237, 197
	, 255, 234, 192
	, 255, 231, 185
	, 255, 227, 178
	, 255, 224, 172
	, 255, 221, 165
	, 255, 217, 158
	, 255, 213, 152
	, 255, 210, 145
	, 255, 206, 138
	, 255, 202, 132
	, 255, 198, 124
	, 255, 194, 118
	, 255, 190, 111
	, 255, 186, 104
	, 255, 182,  97
	, 255, 178,  91
	, 255, 174,  84
	, 255, 171,  78
	, 255, 167,  72
	, 255, 162,  66
	, 255, 159,  59
	, 255, 156,  54
	, 255, 152,  48
	, 255, 149,  43
	, 255, 145,  37
	, 255, 142,  33
	, 255, 140,  29
	, 255, 137,  24
	, 255, 133,  20
	, 255, 131,  16
	, 255, 129,  13
	, 255, 127,  10
	, 255, 125,   7
	, 255, 124,   5
	, 255, 122,   3
	, 255, 122,   1
	, 255, 121,   1
};

MeshModifierNode::MeshModifierNode(void)
{
}

MeshModifierNode::~MeshModifierNode(void)
{
}

MStatus MeshModifierNode::initialize()
{
	MStatus s;
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	MFnEnumAttribute  eAttr;

	m_inMesh = tAttr.create( m_inMeshName[0], m_inMeshName[1], MFnMeshData::kMesh );
	tAttr.setStorable(true);
	tAttr.setHidden(false);

	m_outMesh = tAttr.create( m_outMeshName[0], m_outMeshName[1], MFnMeshData::kMesh );
	tAttr.setStorable(true);
	tAttr.setHidden(false);

	m_vertValArray = tAttr.create(m_vertValArrayName[0], m_vertValArrayName[1], MFnData::kPlugin);
	tAttr.setKeyable(true);
	tAttr.setStorable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(false);


	m_norDirArray = tAttr.create(m_norDirArrayName[0], m_norDirArrayName[1], MFnData::kPlugin);
	tAttr.setKeyable(true);
	tAttr.setStorable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(false);


	m_vertPosArray = tAttr.create(m_vertPosArrayName[0], m_vertPosArrayName[1], MFnData::kPlugin);
	tAttr.setKeyable(true);
	tAttr.setStorable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(false);

	m_vertColorSource = eAttr.create(m_vertColorSourceName[0], m_vertColorSourceName[1]);
	eAttr.addField("From Vertex Value", FROM_VERTEX_VALUE);
	eAttr.addField("From Color Array",  FROM_COLOR_ARRAY);
	eAttr.setHidden(false);
	eAttr.setReadable(true);
	eAttr.setWritable(true);


	m_vertColorArray = tAttr.create(m_vertColorArrayName[0], m_vertColorArrayName[1], MFnData::kPlugin);
	tAttr.setKeyable(true);
	tAttr.setStorable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(false);

	m_autoRange = nAttr.create(m_autoRangeName[0], m_autoRangeName[1], MFnNumericData::kBoolean,1);
	nAttr.setKeyable(true);

	m_minValue = nAttr.create(m_minValueName[0], m_minValueName[1], MFnNumericData::kDouble,0,&s);
	CHECK_STATUS(s);
	nAttr.setKeyable(true);	
	nAttr.setSoftMin(-10);
	nAttr.setSoftMax(10);

	m_maxValue = nAttr.create(m_maxValueName[0], m_maxValueName[1], MFnNumericData::kDouble,0,&s);
	CHECK_STATUS(s);
	nAttr.setKeyable(true);
	nAttr.setSoftMin(-10);
	nAttr.setSoftMax(10);

	s = addAttribute(m_inMesh);
	CHECK_STATUS(s);
	s = addAttribute(m_outMesh);
	CHECK_STATUS(s);
	s = addAttribute(m_autoRange);
	CHECK_STATUS(s);
	s = addAttribute(m_minValue);
	CHECK_STATUS(s);
	s = addAttribute(m_maxValue);
	CHECK_STATUS(s);
	s = addAttribute(m_vertValArray);
	CHECK_STATUS(s);
	s = addAttribute(m_vertColorArray);
	CHECK_STATUS(s);
	s = addAttribute(m_norDirArray);
	CHECK_STATUS(s);
	s = addAttribute(m_vertPosArray);
	CHECK_STATUS(s);
	s = addAttribute(m_vertColorSource);
	CHECK_STATUS(s);

	s = attributeAffects( m_inMesh, m_outMesh );
	CHECK_STATUS(s);
	s = attributeAffects( m_autoRange, m_outMesh);
	CHECK_STATUS(s);
	s = attributeAffects( m_minValue, m_outMesh);
	CHECK_STATUS(s);
	s = attributeAffects( m_maxValue, m_outMesh);
	CHECK_STATUS(s);
	s = attributeAffects( m_vertValArray, m_outMesh);
	CHECK_STATUS(s);
	s = attributeAffects( m_vertColorArray, m_outMesh);
	CHECK_STATUS(s);
	s = attributeAffects( m_norDirArray, m_outMesh);
	CHECK_STATUS(s);
	s = attributeAffects( m_vertPosArray, m_outMesh);
	CHECK_STATUS(s);
	s = attributeAffects( m_vertColorSource, m_outMesh);
	CHECK_STATUS(s);

	return s;
}

MStatus MeshModifierNode::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus s;
	if (plug == m_outMesh)
	{
		MObject nodeObj = thisMObject();
		MFnDependencyNode nodeFn(nodeObj, &s);
		CHECK_STATUS(s);
		MPlug norDirArrayPlug = Global::getPlug(this, m_norDirArrayName[0]);

		// retrieve model data
		MDataHandle inMeshHnd = data.inputValue(m_inMesh,&s);
		CHECK_STATUS(s);
		MObject     inMeshObj  = inMeshHnd.asMesh();
		MFnMeshData meshData;
		MObject     outMeshObj = meshData.create();

		MFnMesh     outMeshFn;
		outMeshFn.copy(inMeshObj, outMeshObj,&s);
		CHECK_STATUS(s);
		outMeshFn.setObject(outMeshObj);
		modifyVertexColor(outMeshObj);
		modifyVertexNormal(outMeshObj);
		modifyVertexPosition(outMeshObj);
		outMeshFn.updateSurface();

		MDataHandle outMeshHnd = data.outputValue(m_outMesh);
		outMeshHnd.set(outMeshObj);
		data.setClean(plug);
	}
	return s;
}

MStatus MeshModifierNode::modifyVertexColor(MObject&outMeshObj)
{
	MStatus s;

	MPlug minValuePlug =     Global::getPlug(this, m_minValueName[0]);
	MPlug maxValuePlug =     Global::getPlug(this, m_maxValueName[0]);
	MPlug vertValArrayPlug=  Global::getPlug(this, m_vertValArrayName[0]);
	MPlug vertClrSrcPlug  =     Global::getPlug(this, m_vertColorSourceName[0]);
	MPlug vertClrPlug  =     Global::getPlug(this, m_vertColorArrayName[0]);

	if (!vertValArrayPlug.isConnected() && !vertClrPlug.isConnected())
		return MS::kFailure;	

	MFnMesh     outMeshFn;
	outMeshFn.setObject(outMeshObj);
	int nVtx = outMeshFn.numVertices(&s);

	if (vertClrSrcPlug.asShort() == FROM_VERTEX_VALUE)
	{	// get value array
		MObject     valueArrayObj;
		vertValArrayPlug.getValue(valueArrayObj);
		MFnPluginData valueArrayFn(valueArrayObj, &s);
		CHECK_STATUS(s);
		if (valueArrayFn.typeId(&s) != MatlabMatrix::getTypeId())
		{
			MGlobal::displayError(this->name() + MString(":Wrong data type. Need matlabMatrix type."));
			return MS::kFailure;
		}
		MatlabMatrix* pValueArray = (MatlabMatrix*)valueArrayFn.data(&s);
		if (!pValueArray)
		{
			MGlobal::displayError(this->name() + MString(": value array is invalid."));
			return MS::kFailure;
		}
		MatrixData& mat = pValueArray->getMatrix();
		int nRows = mat.getRows();
		int nCols = mat.getCols();
		if ((mat.getMatrixType() != MatrixData::MT_DENSE) || (nRows != nVtx) || (nCols != 1) )
		{
			MGlobal::displayError(this->name() + MString(": Wrong matrix dimension or matrix type. Need dense column vector with length nVtx."));
			return MS::kFailure;
		}
		double* valueData = new double[nRows * nCols];
		mat.getDenseDataDouble(valueData);						// get data

		// get min/max value
		bool isAutoRange = Global::getPlug(this, m_autoRangeName[0]).asBool();
		double maxValue, minValue;
		if (!isAutoRange)
		{
			minValue = minValuePlug.asDouble();
			maxValue = maxValuePlug.asDouble();
		}
		else
		{
			minValue = maxValue = valueData[0];
			for (int i = 1; i < nRows * nCols; ++i)
			{
				minValue = minValue < valueData[i] ? minValue : valueData[i];
				maxValue = maxValue > valueData[i] ? maxValue : valueData[i];
			}
			minValuePlug.setDouble(minValue);
			maxValuePlug.setDouble(maxValue);
		}
		minValuePlug.setLocked(isAutoRange);
		maxValuePlug.setLocked(isAutoRange);

		// modify vert color
		MItMeshVertex it(outMeshObj, &s);
		for (int ithVtx = 0; !it.isDone(&s); it.next(), ++ithVtx)
		{
			double v = valueData[ithVtx];
			int colorPersent = CLAMP_INT(0,99,int((v - minValue) / (maxValue - minValue) * 99));
			const unsigned char* colorPtr = m_colorMap0 + colorPersent * 3;
			MColor c(colorPtr[0]/255.0f, colorPtr[1]/255.0f, colorPtr[2]/255.0f);
			outMeshFn.setVertexColor(c, it.index(&s));
		}
		delete[] valueData;
	}
	else
	{
		MObject     colorArrayObj;
		vertClrPlug.getValue(colorArrayObj);
		MatlabMatrix* pMat = NULL;
		MatlabMatrix::checkAndGetMatrixPtr(colorArrayObj, pMat, MatrixData::MT_DENSE, MatrixData::DT_ALL);
		if (!pMat)
		{
			MGlobal::displayError(this->name() + MString(": value array is invalid."));
			return MS::kFailure;
		}
		MatrixData& mat = pMat->getMatrix();
		int nRows = mat.getRows();
		int nCols = mat.getCols();
		if ((mat.getMatrixType() != MatrixData::MT_DENSE) || (nRows != nVtx) || (nCols != 3) )
		{
			MGlobal::displayError(this->name() + MString(": Wrong matrix dimension or matrix type. Need nVtx * 3 dense matrix."));
			return MS::kFailure;
		}
		double* valueData = new double[nRows * nCols];
		mat.getDenseDataDouble(valueData);						// get data

		// modify vert color
		MItMeshVertex it(outMeshObj, &s);
		for (int ithVtx = 0; !it.isDone(&s); it.next(), ++ithVtx)
		{
			double r = valueData[ithVtx];
			double g = valueData[ithVtx+nVtx];
			double b = valueData[ithVtx+nVtx*2];
			MColor c(r,g,b);
			outMeshFn.setVertexColor(c, it.index(&s));
		}
		delete[] valueData;
	}

	return s;
}

MStatus MeshModifierNode::modifyVertexNormal(MObject& outMeshObj)
{
	MStatus s;
	MPlug norDirArrayPlug=  Global::getPlug(this, m_norDirArrayName[0]);
	if (!norDirArrayPlug.isConnected())
		return MS::kFailure;

	MFnMesh     outMeshFn;
	outMeshFn.setObject(outMeshObj);
	int nVtx = outMeshFn.numVertices(&s);

	// get vector array
	MObject vectorArrayObj;
	norDirArrayPlug.getValue(vectorArrayObj);
	double* vectorData = NULL;

	MFnPluginData vectorArrayFn(vectorArrayObj, &s);
	CHECK_STATUS(s);
	if (vectorArrayFn.typeId(&s) != MatlabMatrix::getTypeId())
	{
		MGlobal::displayError(this->name() + MString(": Wrong data type. Need matlabMatrix type."));
		return MS::kFailure;
	}
	MatlabMatrix* pVectorArray = (MatlabMatrix*)vectorArrayFn.data(&s);
	if (!pVectorArray)
	{
		MGlobal::displayError(this->name() + MString(": vert normal array is invalid."));
		return MS::kFailure;
	}
	MatrixData& vecMat = pVectorArray->getMatrix();
	if ((vecMat.getMatrixType() != MatrixData::MT_DENSE) || (vecMat.getRows() != nVtx) || (vecMat.getCols() != 3))
	{
		MGlobal::displayError(this->name() + MString("Wrong matrix dimension or matrix type. Need dense column vector."));
		return MS::kFailure;
	}

	vectorData = new double[nVtx * 3];
	vecMat.getDenseDataDouble(vectorData);

	MItMeshVertex it(outMeshObj, &s);
	for (int ithVtx = 0; !it.isDone(&s); it.next(), ++ithVtx)
	{
		MVector normal(vectorData[ithVtx], vectorData[ithVtx + nVtx], vectorData[ithVtx + nVtx * 2]);
		outMeshFn.setVertexNormal(normal, it.index());
	}
	delete[] vectorData;
	return s;
}

MStatus MeshModifierNode::modifyVertexPosition(MObject& outMeshObj)
{
	MStatus s;
	MPlug vertPosArrayPlug=  Global::getPlug(this, m_vertPosArrayName[0]);
	if (!vertPosArrayPlug.isConnected())
		return MS::kFailure;

	MFnMesh     outMeshFn;
	outMeshFn.setObject(outMeshObj);
	int nVtx = outMeshFn.numVertices(&s);

	// get vector array
	MObject vectorArrayObj;
	vertPosArrayPlug.getValue(vectorArrayObj);
	double* vectorData = NULL;

	MFnPluginData vectorArrayFn(vectorArrayObj, &s);
	CHECK_STATUS(s);
	if (vectorArrayFn.typeId(&s) != MatlabMatrix::getTypeId())
	{
		MGlobal::displayError(this->name() + MString(": Wrong data type. Need matlabMatrix type."));
		return MS::kFailure;
	}
	MatlabMatrix* pVectorArray = (MatlabMatrix*)vectorArrayFn.data(&s);
	if (!pVectorArray)
	{
		MGlobal::displayError(this->name() + MString(": vert position array is invalid."));
		return MS::kFailure;
	}
	MatrixData& vecMat = pVectorArray->getMatrix();
	if ((vecMat.getMatrixType() != MatrixData::MT_DENSE) || (vecMat.getRows() != nVtx) || (vecMat.getCols() != 3))
	{
		MGlobal::displayError(this->name() + MString(": Wrong matrix dimension or matrix type. Need dense column matrix."));
		return MS::kFailure;
	}

	vectorData = new double[nVtx * 3];
	vecMat.getDenseDataDouble(vectorData);

	MItMeshVertex it(outMeshObj, &s);
	for (int ithVtx = 0; !it.isDone(&s); it.next(), ++ithVtx)
	{
		MVector pos(vectorData[ithVtx], vectorData[ithVtx + nVtx], vectorData[ithVtx + nVtx * 2]);
		outMeshFn.setPoint(it.index(),pos);
	}
	delete[] vectorData;
	return s;
}


