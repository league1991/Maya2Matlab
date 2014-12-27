#ifndef _MatlabPluginNode
#define _MatlabPluginNode
//
// Copyright (C) 
// 
// File: MatlabPluginNode.h
//
// Dependency Graph Node: MatlabPlugin
//
// Author: Maya Plug-in Wizard 2.0
//
#include <stdafx.h>
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

#define  NODE_IN_PLUG_FORMAT			"in%d"
#define  NODE_OUT_PLUG_FORMAT			"out%d"

// static matlab data 
class MatlabEngine
{
public:
	static Engine* getOrOpenEngine();
	static bool    isEngineOpen();
	static bool    openEngine();
	static bool    closeEngine();
private:
	static Engine* m_engine;
};

class MatlabNode : public MPxLocatorNode
{
public:
						MatlabNode();
	virtual				~MatlabNode(); 

	void				postConstructor();

	virtual void draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView:: DisplayStatus );
	virtual bool isBounded() const{return true;}
	virtual MBoundingBox boundingBox() const;

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	static  void*		creator();
	static  MStatus		initialize();

	MStatus             computeMatlab(Engine* eng);

	// The typeid is a unique 32bit indentifier that describes this node.
	// It is used to save and retrieve nodes of this type from the binary
	// file format.  If it is not unique, it will cause file IO problems.
	//
	static	MTypeId		m_id;
	static const char*  m_nodeName;

private:
	void				drawIcon();
	void				drawText();
	void                buildNodeShape(const MString& name);
	bool                sendInputsToMatlab(Engine* eng, MPlug& inputPlug, MStatus* s=NULL);
	bool                receiveOutputFromMatlab(Engine* eng, MPlug& outputPlug, MStatus* s=NULL);
	MString             getInputArrayName(int idx);					// get the matlab name of input matrix
	MString             getOutputArrayName(int idx);				// get the matlab name of output matrix

	// There needs to be a MObject handle declared for each attribute that
	// the node will have.  These handles are needed for getting and setting
	// the values later.
	//
	static  MObject     m_curDirString;		// matlab current directory string
	static  MObject     m_instString;		// matlab instruction string
	static  MObject     m_resultString;		// string that record results
	static  MObject     m_inputMatrixArray;	// array of input  matrix
	static  MObject     m_outputMatrixArray;// array of output matrix
	static  MObject     m_immediateUpdate;	// is update immediately


	char*               m_resultBuffer;		 // string to display result
	static const int    m_resultBufferLength;// result buffer's length
	static const char*  m_instStringName[2];
	static const char*  m_curDirStringName[2];
	static const char*  m_resultStringName[2];
	static const char*  m_inputMatrixArrayName[2];
	static const char*  m_outputMatrixArrayName[2];
	static const char*  m_immediateUpdateName[2];

	static const float  m_matlabIconCurve[];

	// shape
	// bounding box
	MBoundingBox        m_box;
	//QList<QPolygonF>    m_textShapes;
	QPolygonF           m_textShape;
	
	// node object name
	MString             m_objectName;
};

#endif
