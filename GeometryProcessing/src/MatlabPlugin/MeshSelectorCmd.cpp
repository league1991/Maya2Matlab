#include "StdAfx.h"
#include "MeshSelectorCmd.h"

const char* MeshSelectorCmd::m_nodeNameFlag[2] = {"-nodeName", "-n"};
MFn::Type MeshSelectorCmd::m_fnTypeArray[3] = {MFn::kMeshVertComponent, MFn::kMeshEdgeComponent, MFn::kMeshPolygonComponent};

MeshSelectorCmd::MeshSelectorCmd(void)
{
}

MeshSelectorCmd::~MeshSelectorCmd(void)
{ 
}

MSyntax MeshSelectorCmd::newSyntax()
{
	MSyntax syntax;
	MStatus s;
	s = syntax.addFlag(m_nodeNameFlag[1], m_nodeNameFlag[0], MSyntax::kString);
	return syntax;
}

void* MeshSelectorCmd::creator()
{
	return new MeshSelectorCmd;
}

MStatus MeshSelectorCmd::doIt( const MArgList& args )
{
	MStatus s;
	MSyntax syn = syntax();
	MArgDatabase argData(syn, args);

	MSelectionList nodeList;
	if (argData.isFlagSet(m_nodeNameFlag[1], &s))
	{
		MString nodeName;
		s = argData.getFlagArgument(m_nodeNameFlag[1], 0, nodeName);
		s = MGlobal::getSelectionListByName(nodeName, nodeList);
		CHECK_STATUS(s);

		MItSelectionList pSel(nodeList, MFn::kDependencyNode , &s);
		int nValidNodes = 0;
		for (; !pSel.isDone(); pSel.next())
		{
			MObject depNode;
			s=pSel.getDependNode(depNode);
			CHECK_STATUS(s);
			MFnDependencyNode nodeFn(depNode, &s);
			CHECK_STATUS(s);
			MString typeName = nodeFn.typeName(&s);
			CHECK_STATUS(s);
			if (typeName == MeshSelectionLocator::m_nodeName)
			{
				// 找到selection locator节点，执行之
				updateMeshSelector(nodeFn);
				nValidNodes++;
			}
		}
		if (!nValidNodes)
		{
			MGlobal::displayInfo("no meshSelector node selected");
		}
	}
	return s;
}

MStatus MeshSelectorCmd::updateMeshSelector( MFnDependencyNode& nodeFn )
{
	MStatus s;

	// get node selected type
	MeshSelectionLocator* selectorNode = (MeshSelectionLocator*)nodeFn.userNode(&s);
	CHECK_STATUS(s);
	MeshSelectionLocator::SelectionType selType = selectorNode->getSelectionType();
	MFn::Type curTypeFn = m_fnTypeArray[selType];

	// get selection info
	MSelectionList curSelection;
	MGlobal::getActiveSelectionList(curSelection);

	std::vector<int> compIDList;
	MString meshName = "";
	// ITERATE THROUGH EACH "VERTEX COMPONENT" THAT IS CURRENTLY SELECTED
	for (MItSelectionList itComponent(curSelection, curTypeFn); 
		 !itComponent.isDone(); itComponent.next())
	{
		// STORE THE DAGPATH, COMPONENT OBJECT AND MESH NAME OF THE CURRENT VERTEX COMPONENT:
		MObject componentObj;
		MDagPath meshDagPath;
		itComponent.getDagPath(meshDagPath, componentObj);
		MString name = meshDagPath.fullPathName();
		if (meshName != "" && name != meshName)
		{
			MGlobal::displayInfo("more than one shape are selected.");
			return MS::kNotFound;
		}

		// VERTEX COMPONENT HAS TO CONTAIN AT LEAST ONE VERTEX:
		if (componentObj.isNull())
			continue;

		// ITERATE THROUGH EACH "VERTEX" IN THE CURRENT VERTEX COMPONENT:
		switch(curTypeFn)
		{
		case MFn::kMeshVertComponent:
			for (MItMeshVertex vertexIter(meshDagPath, componentObj); !vertexIter.isDone(); vertexIter.next())
				compIDList.push_back(vertexIter.index()+1);
			break;
		case MFn::kMeshEdgeComponent:
			for (MItMeshEdge edgeIter(meshDagPath, componentObj); !edgeIter.isDone(); edgeIter.next())
				compIDList.push_back(edgeIter.index()+1);
			break;
		case MFn::kMeshPolygonComponent:
			for (MItMeshPolygon faceIter(meshDagPath, componentObj); !faceIter.isDone(); faceIter.next())
				compIDList.push_back(faceIter.index()+1);
			break;
		}
	}

	if (compIDList.size() <= 0)
	{
		MGlobal::displayError("Nothing selected.Please check the selection type of the node.");
		return MS::kNotFound;
	}
	selectorNode->setCurSelectionGroup(&compIDList[0], compIDList.size());

	return MS::kSuccess;
}

