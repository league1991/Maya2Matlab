#include "StdAfx.h"
#include "MeshSelectorNode.h"

const MString MeshSelectionLocator::m_nodeName = NODE_MESH_SELECTION_LOCATOR_NAME;
const MTypeId MeshSelectionLocator::m_id(NODE_MESH_SELECTION_LOCATOR_ID);

MObject MeshSelectionLocator::m_indexMatArray;
MObject MeshSelectionLocator::m_selectionType;
MObject MeshSelectionLocator::m_mesh;
MObject MeshSelectionLocator::m_curSelectionGroup;
MObject MeshSelectionLocator::m_transform;
MObject MeshSelectionLocator::m_space;
MObject MeshSelectionLocator::m_indexIntArray;
MObject MeshSelectionLocator::m_indexSource;
MObject MeshSelectionLocator::m_colorArray;
MObject MeshSelectionLocator::m_displaySize;
MObject MeshSelectionLocator::m_currentObj;

const char* MeshSelectionLocator::m_currentObjName[2] = {"currentObjToSelect","curObj"};
const char* MeshSelectionLocator::m_displaySizeName[2]={"displaySize","dispSize"};
const char* MeshSelectionLocator::m_indexSourceName[2] = {"indexSource", "idxSrc"};
const char* MeshSelectionLocator::m_curSelectionGroupName[2]={"currentSelectionGroup","curSelGroup"};
const char* MeshSelectionLocator::m_indexMatArrayName[2] = {"indexArray", "id"};
const char* MeshSelectionLocator::m_selectionTypeName[2] = {"selectionType", "selType"};
const char* MeshSelectionLocator::m_meshName[2] = {"mesh", "mesh"};
const char* MeshSelectionLocator::m_transformName[2] = {"transformMatrix", "transform"}; 
const char* MeshSelectionLocator::m_spaceName[2] = {"space", "space"};
const char* MeshSelectionLocator::m_indexIntArrayName[2] = {"indexIntArray", "idInt"};
const char* MeshSelectionLocator::m_colorArrayName[2] = {"colorArray", "clrArray"};



MStatus MeshSelectionLocator::initialize()
{
	MStatus s;
	MFnTypedAttribute tAttr;
	MFnEnumAttribute  eAttr;
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute mAttr;

	m_mesh = tAttr.create( m_meshName[0], m_meshName[1], MFnMeshData::kMesh );
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setKeyable(false);
	tAttr.setReadable(false);
	tAttr.setWritable(true);

	m_indexMatArray = tAttr.create(m_indexMatArrayName[0], m_indexMatArrayName[1], MFnData::kPlugin);
	tAttr.setKeyable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(true);
	tAttr.setArray(true);
	tAttr.setUsesArrayDataBuilder(true);

	m_curSelectionGroup = nAttr.create(m_curSelectionGroupName[0], m_curSelectionGroupName[1], MFnNumericData::kInt);
	nAttr.setKeyable(true);
	nAttr.setWritable(true);
	nAttr.setReadable(true);
	nAttr.setSoftMin(0);
	nAttr.setSoftMax(8);


	m_displaySize = nAttr.create(m_displaySizeName[0], m_displaySizeName[1], MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setWritable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(4.0);
	nAttr.setSoftMin(0);
	nAttr.setSoftMax(10);

	m_colorArray = nAttr.createColor(m_colorArrayName[0], m_colorArrayName[1]);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setReadable(true); 
	nAttr.setArray(true);
	nAttr.setUsesArrayDataBuilder(true);
	nAttr.setUsedAsColor(true);

	m_indexIntArray = tAttr.create(m_indexIntArrayName[0], m_indexIntArrayName[1], MFnData::kIntArray);
	tAttr.setKeyable(true);
	tAttr.setWritable(true);
	tAttr.setReadable(true);
	tAttr.setArray(true);
	tAttr.setUsesArrayDataBuilder(true);

	m_currentObj = tAttr.create(m_currentObjName[0], m_currentObjName[1], MFnData::kString);
	tAttr.setKeyable(false);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(true); 

	m_selectionType = eAttr.create(m_selectionTypeName[0], m_selectionTypeName[1]);
	eAttr.addField("Vertex", SELECTION_VERTEX);
	eAttr.addField("Edge",   SELECTION_EDGE);
	eAttr.addField("Face",   SELECTION_FACE);
	eAttr.setHidden(false);
	eAttr.setReadable(true);
	eAttr.setWritable(true);

	m_indexSource = eAttr.create(m_indexSourceName[0], m_indexSourceName[1]);
	eAttr.addField("From Interface & Index Int Array", FROM_INT_ARRAY);
	eAttr.addField("From Matlab Array",   FROM_MATLAB_ARRAY);
	eAttr.setHidden(false);
	eAttr.setReadable(true);
	eAttr.setWritable(true);

	m_space = eAttr.create(m_spaceName[0], m_spaceName[1],SPACE_WORLD);
	eAttr.addField("World Space",  SPACE_WORLD);
	eAttr.addField("Object Space", SPACE_OBJECT);
	eAttr.setHidden(false);
	eAttr.setReadable(true);
	eAttr.setWritable(true);

	m_transform = mAttr.create(m_transformName[0], m_transformName[1]);
	mAttr.setHidden(false);
	mAttr.setReadable(false);
	mAttr.setWritable(true);
	mAttr.setKeyable(true);


	CHECK_STATUS(s);
	s = addAttribute( m_mesh);
	CHECK_STATUS(s);
	s = addAttribute( m_indexMatArray);
	CHECK_STATUS(s);
	s = addAttribute( m_selectionType);
	CHECK_STATUS(s);
	s = addAttribute( m_indexSource);
	CHECK_STATUS(s);
	s = addAttribute( m_curSelectionGroup);
	CHECK_STATUS(s);
	s = addAttribute( m_transform);
	CHECK_STATUS(s);
	s = addAttribute( m_space);
	CHECK_STATUS(s);
	s = addAttribute( m_indexIntArray);
	CHECK_STATUS(s);
	s = addAttribute( m_colorArray);
	CHECK_STATUS(s);
	s = addAttribute( m_displaySize);
	CHECK_STATUS(s);
	s = addAttribute( m_currentObj);
	CHECK_STATUS(s);

	//s = attributeAffects(m_indexIntArray, m_indexMatArray);
	//CHECK_STATUS(s);

	return MS::kSuccess;
}

void MeshSelectionLocator::draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView:: DisplayStatus )
{ 
	view.beginGL();
	glPushAttrib(GL_CURRENT_BIT);

	// update data when necessary
	if (m_isDataDirty)
	{
		transferAllData();
		m_isDataDirty = false;
	} 

	view.drawText(this->name(), MPoint(0,0,0));
 	drawIcon();

	MMatrix m = path.inclusiveMatrixInverse();
 	drawElements(m);

	glPopAttrib();
	view.endGL();
}

MBoundingBox MeshSelectionLocator::boundingBox() const
{
	return m_bbox;
}

MStatus MeshSelectionLocator::drawVertices( MObject& mesh, MIntArray& id, MMatrix& transMat)
{
	MStatus s;
	MFnMesh meshFn(mesh, &s);
	CHECK_STATUS(s);
	m_bbox = getIconBBox();

	int nVtx = meshFn.numVertices();
	glPushAttrib(GL_POINT_BIT);
	glPointSize(Global::getPlug(this, m_displaySizeName[0]).asFloat());

	glBegin(GL_POINTS);
	for (unsigned ithElement = 0; ithElement < id.length(); ++ithElement)
	{
		int idx = id[ithElement]-1;
		if (idx < 0 || idx >= nVtx)
			continue;
		MPoint pos;
		meshFn.getPoint(idx, pos, MSpace::kObject);
		pos = pos * transMat;
		m_bbox.expand(pos);
		glVertex3d(pos.x, pos.y, pos.z);
	}
	glEnd();

	glPopAttrib();
	return s;
}

MStatus MeshSelectionLocator::drawEdges( MObject& mesh, MIntArray& id, MMatrix& transMat)
{
	MStatus s;
	MFnMesh meshFn(mesh, &s);
	CHECK_STATUS(s);
	m_bbox = getIconBBox();

	MPointArray pntArray;
	meshFn.getPoints(pntArray, MSpace::kObject);
	int nEdges = meshFn.numEdges();

	glPushAttrib(GL_LINE_BIT);
	glLineWidth(Global::getPlug(this, m_displaySizeName[0]).asFloat());

	glBegin(GL_LINES);
	for (unsigned ithElement = 0; ithElement < id.length(); ++ithElement)
	{
		int idx = id[ithElement]-1;
		if (idx < 0 || idx >= nEdges)
			continue;
		int2 edgeID;
		meshFn.getEdgeVertices(idx, edgeID);
		MPoint p0 = pntArray[edgeID[0]] * transMat;
		MPoint p1 = pntArray[edgeID[1]] * transMat;
		glVertex3d(p0.x, p0.y, p0.z);
		glVertex3d(p1.x, p1.y, p1.z);
		m_bbox.expand(p0);
		m_bbox.expand(p1);
	}
	glEnd();

	glPopAttrib();
	return s;
}

MStatus MeshSelectionLocator::drawFaces( MObject& mesh, MIntArray& id, MMatrix& transMat)
{
	MStatus s;
	MFnMesh meshFn(mesh, &s);
	CHECK_STATUS(s);
	m_bbox = getIconBBox();

	MPointArray pntArray;
	meshFn.getPoints(pntArray, MSpace::kObject);
	int nPolys = meshFn.numPolygons();


	glPushAttrib(GL_POLYGON_BIT|GL_LINE_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(Global::getPlug(this, m_displaySizeName[0]).asFloat());

	for (unsigned ithElement = 0; ithElement < id.length(); ++ithElement)
	{
		int idx = id[ithElement]-1;
		if (idx < 0 || idx >= nPolys)
			continue;
		MIntArray polyVtxIDs;
		meshFn.getPolygonVertices(idx, polyVtxIDs);
		glBegin(GL_POLYGON);
		for (unsigned ithVtx = 0; ithVtx < polyVtxIDs.length(); ++ithVtx)
		{
			MPoint p = pntArray[polyVtxIDs[ithVtx]]* transMat;
			glVertex3d(p.x, p.y, p.z);
			m_bbox.expand(p);
		}
		glEnd();
	}
	glEnd();

	glPopAttrib();
	return s;
}

MStatus MeshSelectionLocator::drawElements(MMatrix& thisMatrix)
{
	MStatus s;

	// get plugs
	MPlug meshPlug = Global::getPlug(this, m_meshName[0]);
	MPlug typePlug = Global::getPlug(this, m_selectionTypeName[0]);
	MPlug idxArrayPlug = Global::getPlug(this, m_indexIntArrayName[0]);

	// get MObjects from plug
	MObject meshObj = meshPlug.asMObject(MDGContext::fsNormal, &s);
	CHECK_EXPRESSION(!s, "No mesh found.");
	short elementType = typePlug.asShort();
	CHECK_STATUS(s);
	MMatrix transMat = getTransformMatrix() * thisMatrix;

	for (int ithPlug = 0; ithPlug < (int)idxArrayPlug.numElements(); ++ithPlug)
	{
		MColor color;
		MPlug idxPlug;
		s = getIthIntPlugAndColor(ithPlug, idxPlug, color);
		CHECK_STATUS(s);

		MObject idxObj = idxPlug.asMObject(MDGContext::fsNormal, &s);

		// get int array
		MFnIntArrayData idxFn(idxObj, &s);
		CHECK_STATUS(s);
		MIntArray       idx;
		idxFn.copyTo(idx);

		// draw elements
		glColor3f(color.r, color.g, color.b);
		switch(elementType)
		{
		case SELECTION_VERTEX:
			drawVertices(meshObj, idx, transMat); break;
		case SELECTION_EDGE:
			drawEdges(meshObj, idx, transMat); break;
		case SELECTION_FACE:
			drawFaces(meshObj, idx, transMat); break;		
		}
	}

	return MS::kSuccess;
}

MStatus MeshSelectionLocator::drawIcon()
{
	const float sqrt3 = 1.732f * 0.5f;
	const float half  = 0.5;
	glBegin(GL_LINE_LOOP);
	glVertex3f(0,0,-1);
	glVertex3f(-sqrt3, 0, -half);
	glVertex3f(-sqrt3, 0, half);
	glVertex3f(0     , 0, 1);
	glVertex3f(sqrt3,  0, half);
	glVertex3f(sqrt3,  0, -half);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(-sqrt3, 0, -half);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex3f(0,0,0);
	glVertex3f(0,0,1);
	glVertex3f(sqrt3,0, half);
	glVertex3f(sqrt3,0, -half);
	glEnd();

	return MS::kSuccess;
}

MeshSelectionLocator::MeshSelectionLocator():MPxLocatorNode()
{
	m_bbox = getIconBBox();
	m_isDataDirty = false;
}

MStatus MeshSelectionLocator::setCurSelectionGroup(const int* const idx, int nElements )
{
	MPlug idxPlug;
	MStatus s;


	if(FROM_INT_ARRAY != (SelectionSource)Global::getPlug(this, m_indexSourceName[0]).asShort())
	{
		MGlobal::displayError(this->name() + MString("Please set index source to 'From Interface & Index Int Array' first."));
		return MS::kFailure;
	}


	// find current plug
	s = getCurSelectionIntPlug(idxPlug);
	CHECK_EXPRESSION(!s,"Can't find such group.")

	MIntArray intArray(idx, nElements);
	MFnIntArrayData intFn;
	MObject   valObj = intFn.create(intArray,&s);
	CHECK_STATUS(s);

	// set object
	idxPlug.setValue(valObj);
	return s;
}

MStatus MeshSelectionLocator::getCurSelectionIntPlug( MPlug& idxPlug )
{
	MStatus s;

	// get plugs
	MPlug idxArrayPlug  = Global::getPlug(this, m_indexIntArrayName[0]);
	MPlug curGroupPlug  = Global::getPlug(this, m_curSelectionGroupName[0]);

	// get MObjects from plug
	int curGroupId = curGroupPlug.asInt();
	CHECK_STATUS(s);
	for (unsigned i = 0; i < idxArrayPlug.numElements(&s); i++)
	{
		MPlug ithPlug = idxArrayPlug.elementByPhysicalIndex(i, &s);
		CHECK_STATUS(s);
		int logicalIdx = ithPlug.logicalIndex(&s);
		if (logicalIdx == curGroupId)
		{
			idxPlug = ithPlug;
			return MS::kSuccess;
		}
	}
	return MS::kNotFound;
}

MeshSelectionLocator::SelectionType MeshSelectionLocator::getSelectionType()
{
	MPlug typePlug = Global::getPlug(this, m_selectionTypeName[0]);
	return (MeshSelectionLocator::SelectionType)typePlug.asShort();
}

MMatrix MeshSelectionLocator::getTransformMatrix()
{
	MStatus s;
	SpaceType space = (SpaceType)Global::getPlug(this, m_spaceName[0]).asShort();
	switch(space)
	{
	case SPACE_OBJECT:
		return MMatrix::identity;
	case SPACE_WORLD: 
		MFnMatrixData objMatFn(Global::getPlug(this, m_transformName[0]).asMObject());
// 		MPlug   invMatPlug = Global::getPlug(this, "worldMatrix");
// 		MPlug   invMatPlug0 = invMatPlug.elementByLogicalIndex(0,&s);
// 		MObject invMatObj   = invMatPlug0.asMObject(MDGContext::fsNormal, &s);
// 		MFnMatrixData locatorInvMatFn(invMatObj);
// 		if (!s)	return MMatrix::identity; 
// 		MMatrix locatorInvMat = locatorInvMatFn.matrix(&s);
// 		if (!s)	return MMatrix::identity;	
		return objMatFn.matrix();
	}
	return MMatrix::identity;
}

MBoundingBox MeshSelectionLocator::getIconBBox()
{
	MBoundingBox box;
	box.clear();
	box.expand(MPoint(1,0.1,1));
	box.expand(MPoint(-1,-0.1,-1));
	return box;
}

MStatus MeshSelectionLocator::compute( const MPlug& plug, MDataBlock& data )
{
	return transferAllData();
}

MStatus MeshSelectionLocator::transferData( MPlug& idxMatPlug, MPlug& idxIntPlug, SelectionSource srcType )
{
	MStatus s;
	if (srcType == FROM_INT_ARRAY)
	{
		// get idx from intArray
		MObject idxIntObj = idxIntPlug.asMObject();
		MFnIntArrayData idxIntFn(idxIntObj, &s);
		CHECK_STATUS(s);
		MIntArray idx;
		idxIntFn.copyTo(idx);

		// set idx to matlabMatrix
		MFnPluginData valFn;
		MObject valObj = valFn.create(MatlabMatrix::getTypeId(), &s);
		CHECK_STATUS(s);
		MatlabMatrix* pMat = (MatlabMatrix*)valFn.data(&s);
		CHECK_STATUS(s);
		CHECK_EXPRESSION(!pMat,"can't get matlabMatrix");

		// get matrix data
		CHECK_EXPRESSION(idx.length() == 0, "matrix is empty.");
		int* idxPtr = new int[idx.length()];
		for (unsigned i = 0; i < idx.length(); ++i)	
			idxPtr[i] = idx[i];
		pMat->getMatrix().setDenseDataPtr(idxPtr, idx.length(), 1, MatrixData::DT_INT32);
		idxMatPlug.setValue(valObj);
	}
	else if (srcType == FROM_MATLAB_ARRAY)
	{
		// get data from matlab matrix
		MatlabMatrix* pMatlabMat = NULL;
		MObject matObj  = idxMatPlug.asMObject();
		s = MatlabMatrix::checkAndGetMatrixPtr(matObj, pMatlabMat, MatrixData::MT_DENSE);
		CHECK_STATUS(s);
		
		MatrixData& pMat = pMatlabMat->getMatrix();
		size_t r = pMat.getRows();
		size_t c = pMat.getCols();
		if (r <= 0 || c <= 0)
			return MS::kFailure;
		int * pData = new int[r*c];
		pMat.getDenseDataInt(pData);

		// set data to int array
		MIntArray intArray(pData, r*c);
		delete[] pData;
		MFnIntArrayData intFn;
		MObject   valObj = intFn.create(intArray,&s);

		// set object
		s = idxIntPlug.setMObject(valObj);
	}
	return s;
}

MStatus MeshSelectionLocator::transferAllData()
{
	MStatus s;
	MPlug idxIntPlugArray = Global::getPlug(this, m_indexIntArrayName[0]);
	MPlug idxMatPlugArray = Global::getPlug(this, m_indexMatArrayName[0]);
	SelectionSource srcType = (SelectionSource)Global::getPlug(this, m_indexSourceName[0]).asShort();
	for (unsigned ithMatPlug = 0; ithMatPlug < idxMatPlugArray.numElements(&s); ithMatPlug++)
	{
		MPlug idxMatPlug = idxMatPlugArray.elementByPhysicalIndex(ithMatPlug, &s);
		if (!s)continue;
		int logicalIdx   = idxMatPlug.logicalIndex(&s);
		if (!s)continue;
		MPlug idxIntPlug = idxIntPlugArray.elementByLogicalIndex(logicalIdx, &s);
		if (!s)continue;

		s = transferData(idxMatPlug, idxIntPlug, srcType);
		if (!s)continue;
	}
	return s;
}

MStatus MeshSelectionLocator::setDependentsDirty( MPlug const & inPlug, MPlugArray & affectedPlugs )
{
	m_isDataDirty = true;
	// 		if ( inPlug.attribute() != inputAttr ) {
	// 			return MS::kSuccess;
	// 		}
	// 
	// 		MPlug outArrayPlug(thisMObject(),outputAttr);
	// 
	// 		if (inPlug.isElement()) {
	// 			// First dirty the output output element first.
	// 			// Of course, dirty output element itself
	// 			MPlug elemPlug = outArrayPlug.elementByLogicalIndex(
	// 				inPlug.logicalIndex());
	// 			affectedPlugs.append(elemPlug);
	// 
	// 			// We also need to dirty the parent.
	// 			//
	// 			affectedPlugs.append(outArrayPlug);
	// 		} else {
	// 			// Mark the parent output plug as dirty.
	// 			//
	// 			affectedPlugs.append(outArrayPlug);
	// 
	// 			// Also visit each element.
	// 			//
	// 			unsigned int i,n = outArrayPlug.numElements();
	// 			for (i = 0; i < n; i++) {
	// 				MPlug elemPlug = outArrayPlug.elementByPhysicalIndex(i);
	// 				affectedPlugs.append(elemPlug);
	// 			}
	//		}
	return MS::kSuccess;
}

MStatus MeshSelectionLocator::getIthIntPlugAndColor( int physicalIdx, MPlug& idxPlug, MColor& color )
{
	MStatus s;
	// get plugs
	MPlug idxArrayPlug   = Global::getPlug(this, m_indexIntArrayName[0]);
	MPlug colorArrayPlug = Global::getPlug(this, m_colorArrayName[0]);

	idxPlug = idxArrayPlug.elementByPhysicalIndex(physicalIdx, &s);
	int logicalIdx  = idxPlug.logicalIndex(&s);
	MPlug colorPlug = colorArrayPlug.elementByLogicalIndex(logicalIdx);
	color = Global::getColor(colorPlug, &s);
	return s;
}







