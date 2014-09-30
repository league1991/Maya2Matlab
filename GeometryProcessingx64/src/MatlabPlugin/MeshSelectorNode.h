#pragma once

class MeshSelectionLocator :public MPxLocatorNode
{ 
public:
	enum SelectionType{SELECTION_VERTEX=0, SELECTION_EDGE = 1, SELECTION_FACE = 2};
	enum SpaceType{SPACE_OBJECT = 0, SPACE_WORLD=1};
	enum SelectionSource{FROM_INT_ARRAY = 0, FROM_MATLAB_ARRAY = 1};

	MeshSelectionLocator();
	~MeshSelectionLocator(){}

	virtual void draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView:: DisplayStatus );
	virtual bool isBounded() const{return true;}
	virtual MBoundingBox boundingBox() const;

	MStatus setCurSelectionGroup(const int* const idx, int nElements);
	SelectionType getSelectionType();
	//virtual MStatus	compute( const MPlug& plug, MDataBlock& data );
	static void* creator(){return new MeshSelectionLocator;}
	static MStatus initialize();
	virtual MStatus	compute( const MPlug& plug, MDataBlock& data );

	MStatus transferAllData();
	MStatus transferData(MPlug& idxMatPlug, MPlug& idxIntPlug, SelectionSource srcType);
 
	static const MTypeId m_id;
	static const MString m_nodeName;

	// use this function to record dirty status of data and avoid always call transAllData() when update
	MStatus setDependentsDirty(MPlug const & inPlug, MPlugArray  & affectedPlugs);

private:
	MBoundingBox getIconBBox();
	MMatrix getTransformMatrix();
	MStatus getCurSelectionIntPlug(MPlug& idxPlug);
	MStatus getIthIntPlugAndColor(int physicalIdx, 
		                          MPlug& idxPlug, MColor& color);
	// drawing functions
	MStatus drawIcon();
	MStatus drawElements(MMatrix& thisMatrix);
	MStatus drawVertices( MObject& mesh, MIntArray& id, MMatrix& transform);
	MStatus drawEdges( MObject& mesh, MIntArray& id, MMatrix& transform);
	MStatus drawFaces( MObject& mesh, MIntArray& id, MMatrix& transform);

	// bounding box
	MBoundingBox m_bbox;

	bool         m_isDataDirty;

	// attributes
	static MObject      m_mesh;
	static MObject		m_selectionType;
	static MObject      m_indexSource;
	static MObject		m_indexMatArray;
	static MObject		m_indexIntArray;
	static MObject      m_curSelectionGroup;
	static MObject		m_transform;
	static MObject		m_space;
	static MObject      m_colorArray;
	static MObject      m_displaySize;
	static MObject      m_currentObj;
	
	static const char*  m_meshName[2];
	static const char*  m_selectionTypeName[2];
	static const char*  m_indexSourceName[2];
	static const char*  m_indexMatArrayName[2];
	static const char*  m_indexIntArrayName[2];
	static const char*  m_curSelectionGroupName[2];
	static const char*  m_transformName[2];
	static const char*  m_spaceName[2];
	static const char*  m_colorArrayName[2];
	static const char*  m_displaySizeName[2];
	static const char*  m_currentObjName[2];
};
