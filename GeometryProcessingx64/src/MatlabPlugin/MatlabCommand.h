#pragma once
/***********************************************************************
   MEL command: matlabEngine   
   -open			-o	open matlab engine
   -close			-c	close matlab engine
   -execCmd			-ec	execute a matlab command
   -execNode		-e  execute a selected or specified(use -name flag) matlab node
   -name			-n	specify a node's name when execute a node
   -closeAfterExec	-ca	close engine after execute a node
/************************************************************************/


class MatlabEngineCmd:public MPxCommand
{
public:
	MatlabEngineCmd(void);
	~MatlabEngineCmd(void);

	MStatus   	doIt( const MArgList& args );

	static void* creator();
	static MSyntax newSyntax();
private: 
	MStatus		execNode(MFnDependencyNode& nodeFn);
	static const char* m_execNodeFlag[2];					// 执行选中节点的命令
	static const char* m_closeAfterExecFlag[2];				// 在执行完毕之后关闭引擎,配合execNodeFlag使用
	static const char* m_nodeNameFlag[2];					// 指定某个名字的节点执行,配合execNodeFlag使用
	static const char* m_openEngineFlag[2];					// 打开引擎
	static const char* m_closeEngineFlag[2];				// 关闭引擎
	static const char* m_execCmdFlag[2];					// 执行字符串命令
	static const int   m_cmdResultLength;					// cmdResult的长度
	static char        m_cmdResult[];						// 保存执行结果的字符串		
};
