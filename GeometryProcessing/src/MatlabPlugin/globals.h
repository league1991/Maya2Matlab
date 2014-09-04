#pragma once

#define CHECK_STATUS(x) if(!x){MGlobal::displayError(x.errorString());return x;}
#define CHECK_EXPRESSION(x,str)if(x){MGlobal::displayError(str);return MS::kFailure;}
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define CLAMP_INT(minValue,maxValue,value) (MAX(int(minValue),MIN(int(value),int(maxValue))))
#define CLAMP_FLOAT(minValue,maxValue,value) (MAX(float(minValue),MIN(float(value),float(maxValue))))
 
#define DATA_STREAM_VERSION    QDataStream::Qt_4_0

#define CMD_MATLAB_ENGINE		"matlabEngine"
#define CMD_MESH_SELECTOR		"locateMeshSelection"
#define CMD_EXE_ENGINE			"exeEngine"

#define DATA_MATLAB_MATRIX_NAME		"matlabMatrix"
#define DATA_MATLAB_MATRIX_ID		4400003

#define DATA_MATLAB_STRUCT_NAME		"matlabStruct"
#define DATA_MATLAB_STRUCT_ID		4400010

#define NODE_MATLAB_NAME			"matlab"
#define NODE_MATLAB_ID				4400002

#define NODE_MESH_TO_MATRIX_NAME	"mesh2Matrix"
#define NODE_MESH_TO_MATRIX_ID		4400004

#define NODE_MODEL_PAINTER_NAME		"meshModifier"
#define NODE_MODEL_PAINTER_ID		4400005

#define NODE_MESH_INTERPOLATION_NAME "meshInterpolation"
#define NODE_MESH_INTERPOLATION_ID	4400006

#define NODE_DATA_TO_MATRIX_NAME	"data2Matrix"
#define NODE_DATA_TO_MATRIX_ID		4400007

#define NODE_MESH_SELECTION_LOCATOR_NAME  "meshSelectionLocator"
#define NODE_MESH_SELECTION_LOCATOR_ID    4400008

#define NODE_EXE_CLIENT_NAME		"exeClient"
#define NODE_EXE_CLIENT_ID			4400009

#define NODE_VECTOR_DISPLAY_NAME	"vectorDisplay"
#define NODE_VECTOR_DISPLAY_ID		4400011

#define NODE_MATRIX_TO_DATA_NAME	"matrix2Data"
#define NODE_MATRIX_TO_DATA_ID		4400012

class Global
{
public:
	static MPlug getPlug(MPxNode* node, const char* longName)
	{		
		MObject nodeObj = node->thisMObject();
		MFnDependencyNode nodeFn(nodeObj);
		return nodeFn.findPlug(longName, true);
	}

	static MColor getColor(const MPlug& plug, MStatus* s = NULL)
	{
		MColor color;
		int nC = plug.numChildren(s);
		if (nC == 3)
		{
			float r = plug.child(0).asFloat();
			float g = plug.child(1).asFloat();
			float b = plug.child(2).asFloat();
			color = MColor(r,g,b);
		}
		else if (s)
			*s = MS::kFailure;
		return color;
	}

	static void displayError(const MPxNode* node, const MString& errorMsg)
	{
		MGlobal::displayError(node->name() + ":\t" + errorMsg);
	}
};
