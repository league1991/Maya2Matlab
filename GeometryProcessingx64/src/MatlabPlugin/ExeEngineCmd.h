#pragma once
/***********************************************************************
   MEL command: exeEngine
   -execNode		-e  execute a selected or specified(use -name flag) exeClient node
   -name			-n	specify a node's name when execute a node
/************************************************************************/

class ExeEngineCmd: public MPxCommand
{
public:
	ExeEngineCmd(void);
	~ExeEngineCmd(void);

	MStatus   	   doIt( const MArgList& args );

	static void*   creator(){return new ExeEngineCmd;}
	static MSyntax newSyntax();
private:
	MStatus execNode( MFnDependencyNode& nodeFn );

	static const char* m_execNodeFlag[2];
	static const char* m_nodeNameFlag[2];
};
