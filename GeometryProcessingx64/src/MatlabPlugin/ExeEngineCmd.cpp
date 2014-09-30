#include "StdAfx.h"
#include "ExeEngineCmd.h"

const char* ExeEngineCmd::m_nodeNameFlag[2] = {"-name", "-n"};
const char* ExeEngineCmd::m_execNodeFlag[2] = {"-execNode", "-e"};

ExeEngineCmd::ExeEngineCmd(void)
{
}

ExeEngineCmd::~ExeEngineCmd(void)
{
}

MSyntax ExeEngineCmd::newSyntax()
{
	MSyntax syntax;
	MStatus s;
	s = syntax.addFlag(m_execNodeFlag[1],    m_execNodeFlag[0],    MSyntax::kNoArg);
	s = syntax.addFlag(m_nodeNameFlag[1],    m_nodeNameFlag[0],    MSyntax::kString);
	return syntax;
}

MStatus ExeEngineCmd::doIt( const MArgList& args )
{
	MStatus s;

	// extract arguments
	MSyntax syn = syntax();
	MArgDatabase argData(syn, args);
	if (argData.isFlagSet(m_execNodeFlag[1], &s))
	{
		// 执行某个节点
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

			// 找到matlab 节点，执行之
			if (typeName == NODE_EXE_CLIENT_NAME && execNode(nodeFn))
				nValidNodes++;
		}
		if (!nValidNodes)
		{
			MGlobal::displayInfo("no matlab node selected");
		}
	}
	return s;
}

MStatus ExeEngineCmd::execNode( MFnDependencyNode& nodeFn )
{
	MStatus s;
	ExeClientNode* exeNode = (ExeClientNode*)nodeFn.userNode(&s);
	CHECK_STATUS(s);

	s = exeNode->computeExe();

	if (!s)
		MGlobal::displayError(exeNode->name() +": error occurs with executing EXE engine");
	return s;
}
