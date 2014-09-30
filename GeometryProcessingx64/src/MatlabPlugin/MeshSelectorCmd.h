#pragma once
/***********************************************************************
	MEL command: selector
	-nodeName	-n	specify nodeName to receive the selected data
/************************************************************************/
class MeshSelectorCmd:public MPxCommand
{
public:
	MeshSelectorCmd(void);
	~MeshSelectorCmd(void);

	MStatus   	   doIt( const MArgList& args );

	static void*   creator();
	static MSyntax newSyntax();
private: 
	MStatus updateMeshSelector(MFnDependencyNode& nodeFn);       
	static const char* m_nodeNameFlag[2];
	static MFn::Type   m_fnTypeArray[3];
};
