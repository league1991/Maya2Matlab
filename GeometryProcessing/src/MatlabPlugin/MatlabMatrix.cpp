#include "StdAfx.h"
#include "MatlabMatrix.h"

const char* MatlabMatrix::m_name = DATA_MATLAB_MATRIX_NAME;
const char* MatlabStruct::m_name = DATA_MATLAB_STRUCT_NAME;

const MTypeId MatlabMatrix::m_id(DATA_MATLAB_MATRIX_ID);
const MTypeId MatlabStruct::m_id(DATA_MATLAB_STRUCT_ID);

MatlabMatrix::MatlabMatrix(void):m_D()
{
}

MatlabMatrix::~MatlabMatrix(void)
{
	clear();
}

void MatlabMatrix::clearData()
{
	m_D.clear();
}




void MatlabMatrix::copy( const MPxData &src )
{
	clear();
	const MatlabMatrix& s = (const MatlabMatrix&)src;
	m_D = s.m_D;
}


MStatus MatlabMatrix::readASCII( const MArgList &argList, unsigned int &endOfTheLastParsedElement )
{
//	MGlobal::displayInfo("readASCII");
	int length = argList.length();

	MStatus s;
	int p = endOfTheLastParsedElement;
	MString typeStr = argList.asString(p++, &s);
	CHECK_EXPRESSION(!s, "Parse Error");
	if (typeStr == "d" || typeStr == "dense")
	{
		int nRows = argList.asInt(p++, &s);
		int nCols = argList.asInt(p++, &s);
		CHECK_EXPRESSION(!s, "Parse Error");
		CHECK_EXPRESSION((length != (3 + nRows * nCols) || nRows <= 0 || nCols <= 0), "Number of arguments doesn't match.");

		double* d = new double[nRows * nCols];
		for (int ithElement = 0; ithElement < nRows * nCols; ++ithElement)
		{
			d[ithElement] = argList.asDouble(p++, &s);
			if (!s)
			{
				delete[] d;
				return MS::kFailure;
			}
		}
		m_D.setDenseDataPtr(d, nRows, nCols, MatrixData::DT_DOUBLE);
		endOfTheLastParsedElement = p;
	}
	return MS::kSuccess;
}

MStatus MatlabMatrix::writeBinary( std::ostream& out )
{
	MGlobal::displayInfo("writeBin");
	//if (!&out || !out.good() /*|| !out.rdbuf()*/)
	//	return MS::kFailure;

//	int i = 1;
//	out.write((char*)&i, sizeof(int));
//	out.write((char*)&m_nRows,    sizeof(int));
//	out.write((char*)&m_nCols,    sizeof(int));
/*	if (m_nRows != 0 && m_nCols != 0)
	{
		out.write((char*)m_voidData, m_nRows * m_nCols * getByteCount(m_dataType));
	}*/
	MStatus s = out.fail() ? MS::kFailure : MS::kSuccess;
	return s;
}

MStatus MatlabMatrix::readBinary( std::istream &in, unsigned int length )
{
	MGlobal::displayInfo("readBin");
	if (!&in || !in.good() || !in.rdbuf())
		return MS::kFailure;/*
	clear();
	MStatus s;
	in.read((char*)&m_dataType, sizeof(DataType));
	in.read((char*)&m_nRows,    sizeof(int));
	in.read((char*)&m_nCols,    sizeof(int));
	int bufLength = m_nRows * m_nCols * getByteCount(m_dataType);
	if (bufLength != 0)
	{
		m_voidData = new char[m_nRows * m_nCols];
		in.read((char*)m_voidData, bufLength);
	}
	int restLength = length - (bufLength + sizeof(DataType) + sizeof(int) *2);
	if (restLength != 0)
	{
		in.seekg(restLength, ios::cur);
		MGlobal::displayError("Error occurs when reading MatlabMatrix Data.");
	}*/
	return MS::kSuccess;
}

MStatus MatlabMatrix::writeASCII( std::ostream& out )
{
	MGlobal::displayInfo("writeASCII");
	if (!&out || !out.good() || !out.rdbuf())
		return MS::kFailure;
// 	std::filebuf fb;
// 	fb.open ("test.txt",std::ios::out);
// 	std::ostream os(&fb);
// 	std::ostringstream ostr;
// 	out << "Test sentence\n";
// 	std::ostringstream* ostr = new std::ostringstream;
// 	*ostr << "abcd";
/*	out.rdbuf(ostr->rdbuf());
		bool g= out.good();
		out << "aaa";
		g= out.good();
		int t = out.tellp();
	out << (unsigned)m_dataType << " ";
	out << m_nRows    << " ";
	out << m_nCols    << " ";
	if (m_nRows != 0 && m_nCols != 0)
	{
		switch(m_dataType)
		{
		case DT_FLOAT:
			for (int i = 0; i < m_nRows * m_nCols; ++i)
				out << m_floatData[i] << " ";
			break;
		case DT_DOUBLE:
			for (int i = 0; i < m_nRows * m_nCols; ++i)
				out << m_doubleData[i] << " ";
			break;
		default:
			break;
		}
	}//*/
	return MS::kSuccess;
}





void MatlabMatrix::clear()
{
	m_D.clear();
}

bool MatlabMatrix::sendDataToMatlab( Engine* eng, const MString& varName )
{
	if (m_D.isEmpty())
		return false;
	int l;
	bool res = m_D.sendToEngine(eng, varName.asChar(l));
	return res;
}

bool MatlabMatrix::getDataFromMatlab( Engine* eng, const MString& varName )
{
	clearData();
	int l;
	bool res = m_D.getFromEngine(eng, varName.asChar(l));
	return res;
}


MStatus MatlabMatrix::createMatrixObject( MObject& obj, MatlabMatrix*& matPtr )
{
	MStatus s;
	MFnPluginData valFn;
	obj = valFn.create(MatlabMatrix::getTypeId(), &s);
	CHECK_STATUS(s);
	matPtr = (MatlabMatrix*)valFn.data(&s);
	return s;
}

MStatus MatlabMatrix::checkAndGetMatrixPtr(  MObject&matObj, 
										  MatlabMatrix*& mat, 
										  MatrixData::MatrixType matType,
										  MatrixData::DataType   dataType)
{
	MStatus s;
	MFnPluginData matFn(matObj, &s);

	CHECK_STATUS(s);
	mat = NULL;
	if (matFn.typeId(&s) != MatlabMatrix::getTypeId())
	{
		MGlobal::displayError("Wrong data type. Need matlabMatrix type.");
		return MS::kFailure;
	}
	MatlabMatrix* pMat = (MatlabMatrix*)matFn.data(&s);
	if (!pMat)
	{
		MGlobal::displayError("matrix is invalid.");
		return MS::kFailure;
	}
	MatrixData& matData = pMat->getMatrix();
	if ( !(matData.getMatrixType() & matType))
	{
		MGlobal::displayError("Matrix format(dense/sparse) is wrong.");
		return MS::kFailure;
	}
	if ( !(matData.getDataType() & dataType))
	{
		MGlobal::displayError("Matrix data(float/double/uint8/uint32) is wrong.");
		return MS::kFailure;
	}
	mat = pMat;
	return MS::kSuccess;
}

void* MatlabMatrix::creator()
{
	return new MatlabMatrix;
}



void MatlabStruct::copy( const MPxData &src )
{
	clear();
	const MatlabStruct& s = (const MatlabStruct&)src;
	m_D = s.m_D;
}



bool MatlabStruct::sendDataToMatlab( Engine* eng, const MString& varName )
{
	if (m_D.isEmpty())
		return false;
	int l;
	return m_D.sendToEngine(eng, varName.asChar(l));
}

bool MatlabStruct::getDataFromMatlab( Engine* eng, const MString& varName )
{
	clearData();
	int l;
	return m_D.getFromEngine(eng, varName.asChar(l));
}

MStatus MatlabStruct::createStructObject( MObject& obj, MatlabStruct*& matPtr )
{
	MStatus s;
	MFnPluginData valFn;
	obj = valFn.create(MatlabStruct::getTypeId(), &s);
	CHECK_STATUS(s);
	matPtr = (MatlabStruct*)valFn.data(&s);
	return s;
}

MStatus MatlabStruct::checkAndGetStruct( MObject&matObj, MatlabStruct*& mat )
{
	MStatus s;
	MFnPluginData matFn(matObj, &s);
	CHECK_STATUS(s);
	if (matFn.typeId(&s) != MatlabStruct::getTypeId())
	{
		MGlobal::displayError("Wrong data type. Need matlabMatrix type.");
		return MS::kFailure;
	}
	MatlabStruct* pMat = (MatlabStruct*)matFn.data(&s);
	if (!pMat)
	{
		MGlobal::displayError("matrix is invalid.");
		return MS::kFailure;
	}
	mat = pMat;
	return MS::kSuccess;
}

MatlabStruct::~MatlabStruct (void)
{
	clear();
}

void MatlabStruct::clearData()
{
	m_D.clear();
}

void* MatlabStruct::creator()
{
	return new MatlabStruct;
}


MatlabData::Type MatlabData::getDataType( MObject&matObj )
{
	MStatus s;
	MFnPluginData matFn(matObj, &s);
	if (!s)
		return MATLAB_UNKNOWN;
	if (matFn.typeId(&s) == MatlabStruct::getTypeId() && s)
		return MATLAB_STRUCT;
	if (matFn.typeId(&s) == MatlabMatrix::getTypeId() && s)
		return MATLAB_MATRIX;
	return MATLAB_UNKNOWN;
}

MatlabData::Type MatlabData::getDataType( Engine* eng, const char* varName )
{
	mxArray* matlabArray = engGetVariable(eng, varName);
	MatlabData::Type type = MATLAB_UNKNOWN;

	if (!matlabArray)
		return type;
	if (mxIsStruct(matlabArray))
		type = MATLAB_STRUCT;
	else if (MatrixData::isMatrix(matlabArray))
		type = MATLAB_MATRIX;
	mxDestroyArray(matlabArray);
	return type;
}

MStatus MatlabData::checkAndGetData( MObject&matObj, MatlabData*& dat )
{
	MStatus s;
	MFnPluginData matFn(matObj, &s);

	CHECK_STATUS(s);
	if (matFn.typeId(&s) == MatlabMatrix::getTypeId() && s)
	{
		dat = (MatlabMatrix*)matFn.data(&s);
		return s;
	}
	else if (matFn.typeId(&s) == MatlabStruct::getTypeId() && s)
	{
		dat = (MatlabStruct*)matFn.data(&s);
		return s;
	}
	dat = NULL;
	return s;
}

MatlabData::~MatlabData()
{

}
