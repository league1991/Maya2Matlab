#ifndef _VertexPropertyDisplayNode
#define _VertexPropertyDisplayNode


class VectorDisplayNode : public MPxLocatorNode{
public:
	VectorDisplayNode();
	virtual ~VectorDisplayNode(){};

	virtual void draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView:: DisplayStatus );
	virtual bool isBounded() const{return true;}
	virtual MBoundingBox boundingBox() const;

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	static  void*		creator(){ return new VectorDisplayNode; }
	static  MStatus		initialize();

	static const MTypeId		m_id;
	static const char*	m_nodeName;

private:
	MStatus				drawVectors(const MMatrix& invTransform);
	MStatus				drawIcon();
	MBoundingBox VectorDisplayNode::getBox();
	MMatrix				getTransformMatrix();
	MColor getVectorColor(unsigned idx);
	double getVectorLengthFactor(unsigned idx);
	MStatus drawVectorPerVertex(double* vertCoordData,int nVertRows,int nVertCols,MPlug vecPerVertPlug,MMatrix& transformMatrix,MColor& color,double lenFactor);

private:

	static	MObject		m_verticesCoord;		// #vertices*3		matrix of vertices coordinate
	static	MObject		m_vectorsPerVertex;
	static	MObject		m_transformMatrix;
	static	MObject		m_colorSetting;
	static	MObject		m_vectorLengthArray;
	static	MObject		m_currentIndex;


	static const char*	m_verticesCoordString[2];
	static const char*	m_vectorsPerVertexString[2];
	static const char*	m_transformMatrixString[2];
	static const char*	m_colorSettingString[2];
	static const char*	m_vectorLengthArrayString[2];
	static const char*	m_currentIndexString[2];

	static const float  m_iconShape1[][2];
	static const float  m_iconShape2[][2];

	//shape
	//bounding box
	MBoundingBox		m_box;
};

#endif