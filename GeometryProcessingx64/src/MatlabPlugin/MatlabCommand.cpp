#include "StdAfx.h"
#include "MatlabCommand.h"

const char* MatlabEngineCmd::m_closeEngineFlag[2] = {"-close", "-c"};
const char* MatlabEngineCmd::m_openEngineFlag[2]  = {"-open",  "-o"};
const char* MatlabEngineCmd::m_execNodeFlag[2]    = {"-execNode", "-e"};
const char* MatlabEngineCmd::m_execCmdFlag[2]     = {"-execCmd", "-ec"};
const char* MatlabEngineCmd::m_closeAfterExecFlag[2] = {"-closeAfterExec", "-ca"};
const char* MatlabEngineCmd::m_nodeNameFlag[2]    = {"-name", "-n"};
const int   MatlabEngineCmd::m_cmdResultLength    = 1024 * 20;
char        MatlabEngineCmd::m_cmdResult[m_cmdResultLength];

MatlabEngineCmd::MatlabEngineCmd(void)
{

}

MatlabEngineCmd::~MatlabEngineCmd(void)
{
}

MStatus MatlabEngineCmd::doIt( const MArgList& args )
{
	MStatus s;

	// extract arguments
	MSyntax syn = syntax();
	MArgDatabase argData(syn, args);
	if (argData.isFlagSet(m_openEngineFlag[1], &s))
	{
		// 打开matlab 引擎
		MatlabEngine::openEngine();
		MGlobal::displayInfo("Matlab engine is open.");
		return s;
	}
	else if (argData.isFlagSet(m_closeEngineFlag[1], &s))
	{
		// 关闭matlab 引擎
		MatlabEngine::closeEngine();
		MGlobal::displayInfo("Matlab engine is closed.");
		return s;
	}
	else if (argData.isFlagSet(m_execNodeFlag[1], &s))
	{
		// 执行某个节点
		bool isCloseEngine = false;
		if (argData.isFlagSet(m_closeAfterExecFlag[1], &s))
		{
			s = argData.getFlagArgument(m_closeAfterExecFlag[1],0,isCloseEngine);
		}

		
		MSelectionList selection;
		if (argData.isFlagSet(m_nodeNameFlag[1], &s))
		{
			MString nodeName;											// 提取某个名字的节点
			s = argData.getFlagArgument(m_nodeNameFlag[1],0, nodeName);
			s = MGlobal::getSelectionListByName(nodeName, selection);
		}
		else
			s = MGlobal::getActiveSelectionList(selection);				// 提取选中节点

		MItSelectionList pSel(selection, MFn::kDependencyNode , &s);
		int nValidNodes = 0;
		for (; !pSel.isDone(); pSel.next())
		{
			MObject depNode;
			pSel.getDependNode(depNode);
			MFnDependencyNode nodeFn(depNode, &s);
			MString typeName = nodeFn.typeName(&s);
			if (typeName == NODE_MATLAB_NAME)
			{
				// 找到matlab 节点，执行之
				execNode(nodeFn);
				nValidNodes++;
			}
		}
		if (!nValidNodes)
		{
			MGlobal::displayInfo("no matlab node selected");
		}
		if (isCloseEngine)
		{
			MatlabEngine::closeEngine();
		}
	}
	else if (argData.isFlagSet(m_execCmdFlag[1], &s))
	{
		MString cmd;											
		int cmdLength;
		s = argData.getFlagArgument(m_execCmdFlag[1],0, cmd);	// 提取 matlab 命令
		Engine* eng = MatlabEngine::getOrOpenEngine();
		if (!eng)
		{
			MGlobal::displayError("Failed to find Matlab Engine.");
			return MStatus::kFailure;
		}
		const int resBufferLength = 1000;
		engOutputBuffer(eng, m_cmdResult, m_cmdResultLength);
		int res = engEvalString(eng, cmd.asChar(cmdLength));
		MGlobal::displayInfo(m_cmdResult);
	}
	return s;
}


void* MatlabEngineCmd::creator()
{
	return new MatlabEngineCmd;
}

MSyntax MatlabEngineCmd::newSyntax()
{
	MSyntax syntax;
	MStatus s;
	s = syntax.addFlag(m_openEngineFlag[1], m_openEngineFlag[0], MSyntax::kNoArg);
	s = syntax.addFlag(m_closeEngineFlag[1], m_closeEngineFlag[0], MSyntax::kNoArg);
	s = syntax.addFlag(m_closeAfterExecFlag[1], m_closeAfterExecFlag[0], MSyntax::kBoolean);
	s = syntax.addFlag(m_execNodeFlag[1],    m_execNodeFlag[0],    MSyntax::kNoArg);
	s = syntax.addFlag(m_nodeNameFlag[1],    m_nodeNameFlag[0],    MSyntax::kString);
	s = syntax.addFlag(m_execCmdFlag[1], m_execCmdFlag[0], MSyntax::kString);
	return syntax;
}

MStatus MatlabEngineCmd::execNode( MFnDependencyNode& nodeFn )
{
	MStatus s;
	MatlabNode* matlabNode = (MatlabNode*)nodeFn.userNode(&s);
	CHECK_STATUS(s);

	Engine* eng = MatlabEngine::getOrOpenEngine();
	s = matlabNode->computeMatlab(eng);

	if (!s)
		MGlobal::displayError(matlabNode->name() + MString(": error occurs with executing MATLAB engine"));
	return s;
}

