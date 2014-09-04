//
// Copyright (C) 
// 
// File: pluginMain.cpp
//
// Author: Maya Plug-in Wizard 2.0
//
#include "stdafx.h"
#include "MatlabPluginNode.h"

#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is loaded into Maya.  It 
//		registers all of the services that this plug-in provides with 
//		Maya.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "Ouyang", "1.0", "Any");


	status = plugin.registerData( DATA_MATLAB_MATRIX_NAME, DATA_MATLAB_MATRIX_ID,MatlabMatrix::creator, MPxData::kData);
	if (!status) {MGlobal::displayError("register MatlabMatrix failed");}

	status = plugin.registerData( DATA_MATLAB_STRUCT_NAME, DATA_MATLAB_STRUCT_ID,MatlabStruct::creator, MPxData::kData);
	if (!status) {MGlobal::displayError("register MatlabStruct failed");}

	status = plugin.registerNode( MeshToMatrixNode::m_nodeName, MeshToMatrixNode::m_id, MeshToMatrixNode::creator,
		MeshToMatrixNode::initialize );
	if (!status) {MGlobal::displayError("register meshToMatrix node failed.");}


	status = plugin.registerNode( MatlabNode::m_nodeName, MatlabNode::m_id, MatlabNode::creator,
								  MatlabNode::initialize, MPxNode::kLocatorNode);
	if (!status) {MGlobal::displayError("register matlab node failed.");}

	status = plugin.registerNode( MeshModifierNode::m_nodeName, MeshModifierNode::m_id, MeshModifierNode::creator,
		MeshModifierNode::initialize );
	if (!status) {MGlobal::displayError("register meshModifier node failed.");}


	status = plugin.registerNode( MeshInterpolationNode::m_nodeName, MeshInterpolationNode::m_id, MeshInterpolationNode::creator, MeshInterpolationNode::initialize );
	if (!status) {MGlobal::displayError("register meshInterpolation node failed.");}


	status = plugin.registerNode( DataToMatrixNode::m_nodeName, DataToMatrixNode::m_id, DataToMatrixNode::creator, DataToMatrixNode::initialize );
	if (!status) {MGlobal::displayError("register data2Matrix node failed.");}


	status = plugin.registerNode( MeshSelectionLocator::m_nodeName, MeshSelectionLocator::m_id, MeshSelectionLocator::creator, 
		MeshSelectionLocator::initialize, MPxNode::kLocatorNode);
	if (!status) {MGlobal::displayError("register meshSelector node failed.");}


	status = plugin.registerNode( ExeClientNode::m_nodeName, ExeClientNode::m_id, 
		ExeClientNode::creator,	ExeClientNode::initialize, MPxNode::kLocatorNode);
	if (!status) { 	MGlobal::displayError("register exeEngine node failed.");}


	status = plugin.registerNode( VectorDisplayNode::m_nodeName, VectorDisplayNode::m_id, VectorDisplayNode::creator,
		VectorDisplayNode::initialize,MPxNode::kLocatorNode );
	if (!status) {	MGlobal::displayError("register vectorDisplay node failed.");}


	status = plugin.registerNode( MatrixToDataNode::m_nodeName, MatrixToDataNode::m_id, MatrixToDataNode::creator,
		MatrixToDataNode::initialize );
	if (!status) {	MGlobal::displayError("register matrixToData node failed.");}

	status = plugin.registerCommand(CMD_EXE_ENGINE, ExeEngineCmd::creator, ExeEngineCmd::newSyntax);
	if (!status) {	MGlobal::displayError("register command exeEngine failed."); } 

	status = plugin.registerCommand(CMD_MATLAB_ENGINE, MatlabEngineCmd::creator, MatlabEngineCmd::newSyntax);
	if (!status) {MGlobal::displayError("register command matlabEngine failed."); }

	status = plugin.registerCommand(CMD_MESH_SELECTOR, MeshSelectorCmd::creator, MeshSelectorCmd::newSyntax);
	if (!status) {MGlobal::displayError("register command selector failed."); 	}

	MGlobal::displayInfo("Matlab plugin is loaded successfully!");
LOAD_END:
	return status;
}

MStatus uninitializePlugin( MObject obj)
//
//	Description:
//		this method is called when the plug-in is unloaded from Maya. It 
//		deregisters all of the services that it was providing.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterData(MatlabMatrix::getTypeId());
	if (!status) {
		MGlobal::displayError("deregisterData");
		//return MS::kFailure;
	}

	status = plugin.deregisterData(MatlabStruct::getTypeId());
	if (!status) {
		MGlobal::displayError("deregisterData");
		//return MS::kFailure;
	}


	status = plugin.deregisterNode( MeshToMatrixNode::m_id );
	if (!status) {
		MGlobal::displayError("deregisterNode");
		//return MS::kFailure;
	}

	status = plugin.deregisterNode( MatlabNode::m_id );
	if (!status) {
		MGlobal::displayError("deregisterNode");
		//return MS::kFailure;
	}

	status = plugin.deregisterNode( MeshModifierNode::m_id);
	if (!status) {
		MGlobal::displayError("deregister meshPainterNode node failed.");
		//return MS::kFailure;
	}


	status = plugin.deregisterNode( MeshInterpolationNode::m_id);
	if (!status) {
		MGlobal::displayError("deregister meshInterpolationNode node failed.");
		//return MS::kFailure;
	}


	status = plugin.deregisterNode( DataToMatrixNode::m_id);
	if (!status) {
		MGlobal::displayError("deregister data2Matrix node failed.");
		//return MS::kFailure;
	}

	status = plugin.deregisterNode( MeshSelectionLocator::m_id);
	if (!status) {
		MGlobal::displayError("deregister data2Matrix node failed.");
		//return MS::kFailure;
	}

	status = plugin.deregisterNode( VectorDisplayNode::m_id);
	if (!status) {
		MGlobal::displayError("deregister vectorDisplay node failed.");
		//return MS::kFailure;
	}

	status = plugin.deregisterNode( MatrixToDataNode::m_id);
	if (!status) {
		MGlobal::displayError("deregister matrix2Data node failed.");
		//return MS::kFailure;
	}

	status = plugin.deregisterCommand(CMD_MATLAB_ENGINE);
	if (!status) 
	{
		MGlobal::displayError("deregister command matlabEngine failed."); 
	}

	status = plugin.deregisterCommand(CMD_MESH_SELECTOR);
	if (!status) 
	{
		MGlobal::displayError("deregister command selector failed."); 
	}


	status = plugin.deregisterNode(ExeClientNode::m_id);
	if (!status) 
	{
		MGlobal::displayError("deregister command selector failed."); 
	}

	status = plugin.deregisterCommand(CMD_EXE_ENGINE);
	if (!status) 
	{
		MGlobal::displayError("deregister command selector failed."); 
	}

	MGlobal::displayInfo("Matlab plugin is unloaded successfully!");
	return status;
}
