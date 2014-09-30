
#include <stdlib.h>


#include <maya/MGlobal.h>
#include <maya/MQtUtil.h>

#include <maya/MPxNode.h>
#include <maya/MPxCommand.h>
#include <maya/MPxData.h>
#include <maya/MPxLocatorNode.h>

#include <maya/MIOStream.h>
#include <maya/MDGModifier.h>
#include <maya/MDagModifier.h>
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MPlugArray.h> 
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MPointArray.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h> 
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MMatrix.h>
#include <maya/MArgList.h>

#include <maya/MFnNurbsSurfaceData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnPointArrayData.h>

#include <maya/MItMeshVertex.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItSelectionList.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItMeshPolygon.h>


#include <Eigen/Dense>
#include <eigen/sparse>
#include <Eigen/LU>
#include <Eigen/SVD>
using Eigen::Affine;
using namespace Eigen;

#include <vector>
#include <map>
#include <exception>
using namespace std;
/*
#include <ostream>
#include <istream>
#include <fstream>
#include <sstream>*/
#include <matrix.h>
#include <engine.h>
#include <tmwtypes.h>


#include <QDataStream>
#include <QPolygon>
#include <QHostAddress>
#include <QSharedPointer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QPainterPath>
#include <QNetworkInterface>
#include <QFont>

#include "globals.h"
#include "MatrixData.h"
#include "MatlabMatrix.h"
#include "MatlabPluginNode.h"
#include "MeshToMatrixNode.h"
#include "DataToMatrixNode.h"
#include "MatlabCommand.h"
#include "ModelPainterNode.h"
#include "MeshInterpolationNode.h"
#include "MeshSelectorNode.h"
#include "MeshSelectorCmd.h"
#include "ExePacket.h"
#include "ExeClient.h"
#include "ExeServer.h"
#include "ExeClientNode.h"
#include "ExeEngineCmd.h"
#include "vectordisplaynode.h"
#include "MatrixToDataNode.h"