
#include "StdAfx.h"
#include "MatrixData.h"
#include "serverstdafx.h"
#include "clientstdafx.h"


MatrixData::MatrixData():AbstractData()
{
	m_absType  = AbstractData::AT_MATRIX;
	m_matType  = MT_UNKNOWN;
	m_dataType = DT_UNKNOWN;
	m_voidData = NULL;
	m_rowID    = NULL;
	m_colRange = NULL;
	m_nRows = m_nCols = m_nRowIDs = m_nColRanges = 0;
}

MatrixData::MatrixData( const MatrixData& src ):AbstractData()
{
	m_absType  = AbstractData::AT_MATRIX;
	m_matType  = MT_UNKNOWN;
	m_dataType = DT_UNKNOWN;
	m_voidData = NULL;
	m_rowID    = NULL;
	m_colRange = NULL;
	m_nRows = m_nCols = m_nRowIDs = m_nColRanges = 0;
	*(this) = src;
}

void MatrixData::clear()
{
	if (m_voidData)
		delete[] m_voidData;
	if (m_rowID)
		delete[] m_rowID;
	if (m_colRange)
		delete[] m_colRange;
	m_voidData = NULL;
	m_rowID = NULL;
	m_colRange = NULL;
	m_nRows = m_nCols = m_nRowIDs = m_nColRanges = 0;
	m_matType = MT_UNKNOWN;
	m_dataType = DT_UNKNOWN;
}

MatrixData::~MatrixData()
{
	clear();
}

bool MatrixData::isEmpty()const
{
	return m_voidData == NULL || m_nRows == 0 || m_nCols == 0;
}

MatrixData& MatrixData::operator=( const MatrixData& s )
{
	clear();
	if (s.isEmpty())
		return *this;
	if(s.m_matType == MT_DENSE)
	{
		setDenseData(s.m_voidData, s.m_nRows, s.m_nCols, s.m_dataType);
	}
	else if (s.m_matType == MT_SPARSE)
	{
		setSparseData(s.m_voidData, s.m_rowID, s.m_colRange,
			s.m_nRows, s.m_nCols, s.m_nRowIDs, s.m_nColRanges, s.m_dataType);
	}
	return *this;
}

void MatrixData::setDenseData( const void* const data, size_t nRows, size_t nCols, DataType type )
{
	clear();
	m_nRows = nRows; m_nCols = nCols;
	m_matType = MT_DENSE;
	m_dataType = type;

	size_t size = getByteCount(m_dataType)*m_nRows*m_nCols;
	if (!size)
		return;
	m_voidData = malloc(size);
	memcpy(m_voidData, data, size);
}

void MatrixData::setSparseData(const void* const data, mwIndex* rowID, mwIndex* rowRange, 
							   size_t nRows, size_t nCols, size_t nRowID, size_t nRowRange, 
							   DataType type)
{
	clear();
	m_nRows = nRows; m_nCols = nCols;
	m_nRowIDs = nRowID; m_nColRanges = nRowRange;
	m_matType = MT_SPARSE;
	m_dataType = type;

	size_t size = getByteCount(m_dataType) * m_nRowIDs;
	size_t rowIDSize = sizeof(mwIndex)*m_nRowIDs;
	size_t rowRangeSize = sizeof(mwIndex)*m_nColRanges;
	if (!size || !m_nRowIDs || !m_nColRanges)
		return;

	m_voidData = malloc(size);
	m_rowID    = (mwIndex*)malloc(rowIDSize);
	m_colRange = (mwIndex*)malloc(rowRangeSize);
	memcpy(m_voidData, data, size);
	memcpy(m_rowID, rowID, rowIDSize);
	memcpy(m_colRange, rowRange, rowRangeSize);
}

void MatrixData::setDenseDataPtr(void* const data, size_t nRows, size_t nCols, DataType type )
{
	clear();
	m_nRows = nRows; m_nCols = nCols;
	m_matType = MT_DENSE;
	m_dataType = type;
	m_voidData = data;
}

void MatrixData::setSparseDataPtr( void* const data, mwIndex* rowID, mwIndex* rowRange, size_t nRows, size_t nCols, size_t nRowID, size_t nRowRange, DataType type )
{
	clear();
	m_nRows = nRows; m_nCols = nCols;
	m_rowID = rowID; m_colRange = rowRange; 
	m_nRowIDs = nRowID; m_nColRanges = nRowRange;
	m_matType = MT_SPARSE;
	m_dataType = type;
	m_voidData = data;
}

void MatrixData::getDenseDataDouble(double* data)
{
	int byteSize = getByteCount(m_dataType);
	for (int i = 0; i < m_nRows * m_nCols; ++i)
	{
		switch(m_dataType)
		{
		case DT_FLOAT:
			data[i] = *((float*)((char*)m_voidData + i * byteSize)); break;
		case DT_DOUBLE:
			data[i] = *((double*)((char*)m_voidData + i * byteSize)); break;
		case DT_UINT8:
			data[i] = *((unsigned char*)((char*)m_voidData + i * byteSize)); break;
		case DT_UINT32:
			data[i] = *((unsigned int*)((char*)m_voidData + i * byteSize)); break;
		case DT_INT8:
			data[i] = *((char*)((char*)m_voidData + i * byteSize)); break;
		case DT_INT32:
			data[i] = *((int*)((char*)m_voidData + i * byteSize)); break;
		case DT_BOOL:
			data[i] = *((bool*)((char*)m_voidData + i * byteSize)); break;
		}
	}
}

void MatrixData::getDenseDataInt(int* data)
{
	int byteSize = getByteCount(m_dataType);
	for (int i = 0; i < m_nRows * m_nCols; ++i)
	{
		switch(m_dataType)
		{
		case DT_FLOAT:
			data[i] = (int)*((float*)((char*)m_voidData + i * byteSize)); break;
		case DT_DOUBLE:
			data[i] = (int)*((double*)((char*)m_voidData + i * byteSize)); break;
		case DT_UINT8:
			data[i] = (int)*((unsigned char*)((char*)m_voidData + i * byteSize)); break;
		case DT_UINT32:
			data[i] = (int)*((unsigned int*)((char*)m_voidData + i * byteSize)); break;
		case DT_INT8:
			data[i] = (int)*((char*)((char*)m_voidData + i * byteSize)); break;
		case DT_INT32:
			data[i] = (int)*((int*)((char*)m_voidData + i * byteSize)); break;
		case DT_BOOL:
			data[i] = (int)*((bool*)((char*)m_voidData + i * byteSize)); break;
		}
	}
}

int MatrixData::getByteCount( DataType type )
{ 
	switch(type)
	{
	case DT_FLOAT:
		return sizeof(float);
	case DT_DOUBLE:
		return sizeof(double);
	case DT_INT8:
		return sizeof(char);
	case DT_INT32:
		return sizeof(int);
	case DT_UINT8:
		return sizeof(unsigned char);
	case DT_UINT32:
		return sizeof(unsigned);
	case DT_BOOL:
		return sizeof(bool);
	case DT_UNKNOWN:
	default:
		return 0;
	}
}
#ifdef MATLAB_ENGINE
MatrixData::DataType MatrixData::convertMatlabToNativeType( mxClassID matType )
{
	switch(matType)
	{
	case mxDOUBLE_CLASS:
		return MatrixData::DT_DOUBLE;
	case mxSINGLE_CLASS:
		return MatrixData::DT_FLOAT;
	case mxUINT8_CLASS:
		return MatrixData::DT_UINT8;
	case mxUINT32_CLASS:
		return MatrixData::DT_UINT32;
	case mxINT8_CLASS:
		return MatrixData::DT_INT8;
	case mxINT32_CLASS:
		return MatrixData::DT_INT32;
	case mxLOGICAL_CLASS:
		return MatrixData::DT_BOOL;
	default:
		return MatrixData::DT_UNKNOWN;
	}
}

mxClassID MatrixData::convertNativeToMatlabType( DataType type )
{
	switch(type)
	{
	case MatrixData::DT_DOUBLE:
		return mxDOUBLE_CLASS;
	case MatrixData::DT_FLOAT:
		return mxSINGLE_CLASS;
	case MatrixData::DT_UINT8:
		return mxUINT8_CLASS;
	case MatrixData::DT_UINT32:
		return mxUINT32_CLASS;
	case MatrixData::DT_INT8:
		return mxINT8_CLASS;
	case MatrixData::DT_INT32:
		return mxINT32_CLASS;
	case MatrixData::DT_BOOL:
		return mxLOGICAL_CLASS;
	default:
		return mxUNKNOWN_CLASS;
	}
}



bool MatrixData::sendToArray( mxArray*& m_matlabArray )
{
	if (isEmpty())
		return false;
	if (getMatrixType() == MatrixData::MT_DENSE)
	{
		// set dense matrix data
		void* d;
		size_t row, col;
		MatrixData::DataType type;
		getDenseDataPtr(d, row, col, type);
		m_matlabArray = mxCreateNumericMatrix(row, col, MatrixData::convertNativeToMatlabType(type), mxREAL);
		if (!m_matlabArray || row == 0 || col == 0)
			return false;
		memcpy(mxGetData(m_matlabArray), d, row * col * MatrixData::getByteCount(type));
	}
	else if (getMatrixType() == MatrixData::MT_SPARSE)
	{
		// set sparse matrix data
		void*d;
		mwIndex* rID, *rRange;
		size_t nR, nC, nRID, nRRange;
		MatrixData::DataType type;
		
		getSparseDataPtr(d, rID, rRange, nR, nC, nRID, nRRange, type);
		m_matlabArray = mxCreateSparse(nR, nC, nRID, mxREAL);
		if (!m_matlabArray || nR == 0 || nC == 0 || nRID == 0 || nRRange == 0)
			return false;
		double* ptr = (double*)mxGetPr(m_matlabArray);
		// copy matrix actual data
		switch(type)
		{
		case MatrixData::DT_FLOAT:
			for (size_t i = 0; i < nRID; i++)
				ptr[i] = ((float*)d)[i];
			break;
		case MatrixData::DT_DOUBLE:
			for (size_t i = 0; i < nRID; i++)
				ptr[i] = ((double*)d)[i];
			break;
		case MatrixData::DT_INT8:
			for (size_t i = 0; i < nRID; i++)
				ptr[i] = ((char*)d)[i];
			break;
		case MatrixData::DT_INT32:
			for (size_t i = 0; i < nRID; i++)
				ptr[i] = ((int*)d)[i];
			break;
		case MatrixData::DT_UINT8:
			for (size_t i = 0; i < nRID; i++)
				ptr[i] = ((unsigned char*)d)[i];
			break;
		case MatrixData::DT_UINT32:
			for (size_t i = 0; i < nRID; i++)
				ptr[i] = ((unsigned int*)d)[i];
			break;
		case MatrixData::DT_BOOL:
			for (size_t i = 0; i < nRID; i++)
				ptr[i] = ((unsigned int*)d)[i];
			break;
		case  MatrixData::DT_UNKNOWN:
			return false;
		}

		mwIndex* ir = mxGetIr(m_matlabArray);
		mwIndex* jc = mxGetJc(m_matlabArray);
		for (size_t i = 0; i < nRID; ++i)
			ir[i] = rID[i];
		for (size_t i = 0; i < nRRange; ++i)
			jc[i] = rRange[i];
	}
	return true;
}

bool MatrixData::getFromArray( mxArray* matlabArray )
{
	clear();

	// query array info
	MatrixData::DataType dataType = MatrixData::convertMatlabToNativeType(mxGetClassID(matlabArray));
	bool isSparse       = mxIsSparse(matlabArray);
	mwSize numDim = mxGetNumberOfDimensions(matlabArray);
	if (dataType == MatrixData::DT_UNKNOWN)
		return false;

	const mwSize* dim = mxGetDimensions(matlabArray);
	size_t row, col;
	if (numDim == 1)
	{
		col = 1;		row = dim[0];
	}
	else if (numDim == 2)
	{
		col = dim[1];	row = dim[0];
	}
	else
		return false;
	if (row <= 0 || col <= 0)
		return false;

	// copy data
	if (!isSparse)
	{
		// dense matrix
		void* data = mxGetData(matlabArray);
		size_t byteLength = row * col * MatrixData::getByteCount(dataType);
		if (!byteLength)
			return false;
		setDenseData(data, row, col, dataType);
	}
	else
	{
		// sparse matrix
		void* data = mxGetPr(matlabArray);
		mwIndex * rowID= (mwIndex*)mxGetIr(matlabArray);
		mwIndex * rowRange=(mwIndex*)mxGetJc(matlabArray);
		mwSize length = mxGetNzmax(matlabArray);
		if (!length || !data || !rowID || !rowRange)
			return false;
		setSparseData(data, rowID, rowRange, row, col, length, row + 1, dataType);
	}
	return true;
}


bool MatrixData::isMatrix( mxArray* matlabArray )
{
	DataType dataType = convertMatlabToNativeType(mxGetClassID(matlabArray));
	return dataType != MatrixData::DT_UNKNOWN;
}

#endif

#if EIGEN
//Convert Matrix Data To Eigen Matrix
bool MatrixData::toEigenIntDenseMatrix(Eigen::MatrixXi &matrix){
	if (isEmpty())
		return false;
	if (getMatrixType() == MatrixData::MT_DENSE)
	{
		// set dense matrix data
		void* d;
		size_t row, col;
		MatrixData::DataType type;
		getDenseDataPtr(d, row, col, type);
		if(type != DT_INT32)
			return false;
		matrix = Map<MatrixXi>((int*)d,row,col);
		return true;
	}

	return false;
}

bool MatrixData::toEigenFloatDenseMatrix(Eigen::MatrixXf &matrix){
	if (isEmpty())
		return false;
	if (getMatrixType() == MatrixData::MT_DENSE)
	{
		// set dense matrix data
		void* d;
		size_t row, col;
		MatrixData::DataType type;
		getDenseDataPtr(d, row, col, type);
		if(type != DT_FLOAT)
			return false;
		matrix = Map<MatrixXf>((float*)d,row,col);
		return true;
	}
	return false;
}

bool MatrixData::toEigenDoubleDenseMatrix(Eigen::MatrixXd &matrix){
	if (isEmpty())
		return false;
	if (getMatrixType() == MatrixData::MT_DENSE)
	{
		// set dense matrix data
		void* d;
		size_t row, col;
		MatrixData::DataType type;
		getDenseDataPtr(d, row, col, type);
		if(type != DT_DOUBLE)
			return false;
		matrix = Map<MatrixXd>((double*)d,row,col);
		return true;
	}
	return false;
}

// Convert MatrixData To Eigen Sparse Matrix
bool MatrixData::toEigenIntSparseMatrix(Eigen::SparseMatrix<int> &matrix){
	if (isEmpty())
		return false;
	if (getMatrixType() == MatrixData::MT_SPARSE)
	{
		// set sparse matrix data
		void*d;
		mwIndex* rID, *rRange;
		size_t nR, nC, nRID, nRRange;
		MatrixData::DataType type;
		getSparseDataPtr(d, rID, rRange, nR, nC, nRID, nRRange, type);

		matrix.resize(nR,nC);

		//The simplest way to create a sparse matrix while guaranteeing good performance 
		//is to first build a list of so-called triplets, and then convert it 
		//to a SparseMatrix.
		std::vector<Triplet<int>> tripleList;

		// jc[number of columns], whose array name is called rRange here, 
		// is equal to nnz that is the number of nonzero elements in the entire sparse mxArray
		tripleList.reserve(rRange[nC]);

		int *pData = (int*)d;
		for(size_t i = 0; i < nC; i++)
		{
			// jc[i] is the index in ir of the first nonzero element in the ith column.
			// jc[i+1]-1 is the index of the last nonzero element in the ith column.
			size_t nRow = rRange[i+1] - rRange[i];
			for(size_t j = 0; j < nRow; j++ )
			{
				tripleList.push_back(Triplet<int>((*rID++),i,*pData++));
			}
		}

		matrix.setFromTriplets(tripleList.begin(),tripleList.end());
		return true;

	}
	return false;
}
bool MatrixData::toEigenFloatSparseMatrix(Eigen::SparseMatrix<float> &matrix){
	if (isEmpty())
		return false;
	if (getMatrixType() == MatrixData::MT_SPARSE)
	{
		// set sparse matrix data
		void*d;
		mwIndex* rID, *rRange;
		size_t nR, nC, nRID, nRRange;
		MatrixData::DataType type;
		getSparseDataPtr(d, rID, rRange, nR, nC, nRID, nRRange, type);

		matrix.resize(nR,nC);

		std::vector<Triplet<float>> tripleList;
		// jc[number of columns], whose array name is called rRange here, 
		// is equal to nnz that is the number of nonzero elements in the entire sparse mxArray
		tripleList.reserve(rRange[nC]);

		float *pData = (float*)d;
		for(size_t i = 0; i < nC; i++)
		{
			// jc[i] is the index in ir of the first nonzero element in the ith column.
			// jc[i+1]-1 is the index of the last nonzero element in the ith column.
			size_t nRow = rRange[i+1] - rRange[i];
			for(size_t j = 0; j < nRow; j++ )
			{
				tripleList.push_back(Triplet<float>((*rID++),i,*pData++));
			}
		}
		matrix.setFromTriplets(tripleList.begin(),tripleList.end());
		return true;

	}
	return false;
}
bool MatrixData::toEigenDoubleSparseMatrix(Eigen::SparseMatrix<double> &matrix){
	if (isEmpty())
		return false;
	if (getMatrixType() == MatrixData::MT_SPARSE)
	{
		// set sparse matrix data
		void*d;
		mwIndex* rID, *rRange;
		size_t nR, nC, nRID, nRRange;
		MatrixData::DataType type;
		getSparseDataPtr(d, rID, rRange, nR, nC, nRID, nRRange, type);

		matrix.resize(nR,nC);

		std::vector<Triplet<double>> tripleList;
		// jc[number of columns], whose array name is called rRange here, 
		// is equal to nnz that is the number of nonzero elements in the entire sparse mxArray
		tripleList.reserve(rRange[nC]);

		double *pData = (double*)d;
		for(size_t i = 0; i < nC; i++)
		{
			// jc[i] is the index in ir of the first nonzero element in the ith column.
			// jc[i+1]-1 is the index of the last nonzero element in the ith column.
			size_t nRow = rRange[i+1] - rRange[i];
			for(size_t j = 0; j < nRow; j++ )
			{
				tripleList.push_back(Triplet<double>((*rID++),i,*pData++));
			}
		}
		matrix.setFromTriplets(tripleList.begin(),tripleList.end());
		return true;

	}
	return false;
}

#endif
QDataStream& operator<<( QDataStream &dataStream, const MatrixData& mat )
{
	dataStream << (quint32)mat.m_matType;
	dataStream << (quint32)mat.m_dataType;
	dataStream << mat.m_nRows << mat.m_nCols << mat.m_nRowIDs << mat.m_nColRanges;
	int elementSize = MatrixData::getByteCount(mat.m_dataType);

	if (mat.m_matType == MatrixData::MT_DENSE)
	{
		size_t nBytes = mat.m_nRows * mat.m_nCols * elementSize;
		if (nBytes)
		{
			dataStream.writeRawData((const char*)mat.m_voidData, nBytes);
		}
	}
	else if (mat.m_matType == MatrixData::MT_SPARSE)
	{
		size_t dataBytes = mat.m_nRowIDs * elementSize;
		size_t rowIDBytes = mat.m_nRowIDs * sizeof(mwIndex);
		size_t colRangeBytes = mat.m_nColRanges * sizeof(mwIndex);
		if (dataBytes && rowIDBytes && colRangeBytes)
		{
			dataStream.writeRawData((const char*)mat.m_voidData, dataBytes);
			dataStream.writeRawData((const char*)mat.m_rowID, rowIDBytes);
			dataStream.writeRawData((const char*)mat.m_colRange, colRangeBytes);
		}
	}
	return dataStream;
}

QDataStream& operator>>( QDataStream &dataStream, MatrixData& mat )
{
	mat.clear();
	quint32 matType, dataType;
	qint32  nRows, nCols, nRowIDs, nColRanges;
	dataStream >> matType >> dataType >> nRows >> nCols >> nRowIDs >> nColRanges;
	mat.m_matType = (MatrixData::MatrixType)matType;
	mat.m_dataType = (MatrixData::DataType)dataType;
	mat.m_nRows    = nRows;
	mat.m_nCols    = nCols;
	mat.m_nRowIDs  = nRowIDs;
	mat.m_nColRanges = nColRanges;

	int elementSize = MatrixData::getByteCount(mat.m_dataType);
	mat.m_voidData = NULL;
	mat.m_rowID = mat.m_colRange = NULL;

	bool isSuccess = true;
	if (mat.m_matType == MatrixData::MT_DENSE)
	{
		size_t nBytes = mat.m_nRows * mat.m_nCols * elementSize;
		if (nBytes)
		{
			mat.m_voidData = (void*)new char[nBytes];
			isSuccess = (dataStream.readRawData((char*)mat.m_voidData, nBytes) != -1);
		}
	}
	else if (mat.m_matType == MatrixData::MT_SPARSE)
	{
		size_t dataBytes = mat.m_nRowIDs * elementSize;
		size_t rowIDBytes = mat.m_nRowIDs * sizeof(mwIndex);
		size_t colRangeBytes = mat.m_nColRanges * sizeof(mwIndex);
		if (dataBytes && rowIDBytes && colRangeBytes)
		{
			mat.m_voidData = (void*)new char[dataBytes];
			mat.m_rowID    = new mwIndex[mat.m_nRowIDs];
			mat.m_colRange = new mwIndex[mat.m_nColRanges];

			isSuccess &= (-1 != dataStream.readRawData((char*)mat.m_voidData, dataBytes));
			isSuccess &= (-1 != dataStream.readRawData((char*)mat.m_rowID, rowIDBytes));
			isSuccess &= (-1 != dataStream.readRawData((char*)mat.m_colRange, colRangeBytes));
		}
	}
	if (!isSuccess)
	{
		mat.clear();
	}
	return dataStream;
}

StructData::StructData()
{
	m_absType = AbstractData::AT_STRUCT;
	m_rows = m_cols = 0;
}

StructData::StructData( const StructData& src )
{
	*(this) = src;
}

StructData::~StructData()
{
	clear();
}

StructData& StructData::operator=( const StructData& src )
{
	m_rows = src.m_rows;
	m_cols = src.m_cols;
	m_fields = src.m_fields;
	return *this;
}

void StructData::clear()
{
	m_fields.clear();
}


bool StructData::addField( const QString& fieldName, const FieldData& fieldData )
{
	if (m_rows != fieldData.rows() || m_cols != fieldData.cols())
		return false;

	m_fields[fieldName] = fieldData;
	return true;
}




FieldData* StructData::getField( const QString& fieldName )
{
	if (!m_fields.contains(fieldName))
		return NULL;
	return &m_fields[fieldName];
}

FieldData::ElementPtr StructData::getElement( const QString& fieldName, int row, int col )
{
	if (!m_fields.contains(fieldName))
		return FieldData::ElementPtr();
	return m_fields[fieldName].getElement(row, col);
}

bool StructData::addElement( const QString& fieldName, int row, int col, FieldData::ElementPtr pData )
{
	if (!m_fields.contains(fieldName))
	{
		FieldData fd(fieldName, m_rows, m_cols);
		m_fields[fieldName] = fd;
	}
	FieldData& fieldData = m_fields[fieldName];
	
	return fieldData.setElement(row, col, pData);
}

void StructData::allocateFieldNameArray( char**& nameArray, int& length)
{
	int nNames = m_fields.size();
	nameArray = new char*[nNames];
	length = nNames;

	int ithField = 0;
	for (QHash<QString, FieldData>::ConstIterator pField = m_fields.constBegin();
		pField != m_fields.constEnd(); ++pField, ++ithField)
	{
		QString fieldNameQ = pField.key();
		if (fieldNameQ == "")
			fieldNameQ = QString("unknown%1").arg(ithField);
		QByteArray bytes = fieldNameQ.toAscii();
		int fieldNameLength = bytes.size();

		nameArray[ithField] = new char[fieldNameLength+1];
		memcpy(nameArray[ithField], bytes.data(), sizeof(char) * fieldNameLength);
		nameArray[ithField][fieldNameLength] = '\0';
	}
}

void StructData::releaseFieldNameArray( char**& nameArray, int length )
{
	for (int i = 0; i < length; i++)
	{
		delete nameArray[i];
	}
	delete[] nameArray;
	nameArray = NULL;
}

#ifdef MATLAB_ENGINE
bool StructData::sendToArray( mxArray*& structArray )
{
	if (isEmpty())
		return false;
	char** fieldNames = NULL;
	int    nFields    = m_fields.size();

	int nFieldNames;
	allocateFieldNameArray(fieldNames, nFieldNames);
	structArray = mxCreateStructMatrix(m_rows, m_cols, nFields, (const char**)fieldNames);
	if (structArray == NULL)
		return false;
	for (QHash<QString, FieldData>::Iterator pField = m_fields.begin();
		pField != m_fields.end(); ++pField)
	{
		FieldData& field = pField.value();
		field.sendToArray(structArray);
	}
	releaseFieldNameArray(fieldNames, nFieldNames);
	return true;
}

bool StructData::getFromArray( mxArray* structArray )
{
	clear();
	int nFields = mxGetNumberOfFields(structArray);
	m_rows = mxGetM(structArray);
	m_cols = mxGetN(structArray);
	if (m_rows <= 0 || m_cols <= 0)
	{
		m_rows = m_cols = 0;
		return false;
	}
	if (nFields == 0)
		return false;

	for (int ithField = 0; ithField < nFields; ++ithField)
	{
		const char* name = mxGetFieldNameByNumber(structArray, ithField);
		if (name == NULL)
			continue;

		QString nameQ = QString::fromAscii(name);
		FieldData&   fd = m_fields[nameQ];

		fd.setName(nameQ);
		fd.getFromArray(structArray);
	}
	return true;
}
#endif

FieldData::FieldData( const QString& fieldName, size_t rows /*= 0*/, size_t cols /*= 0*/ )
{
	m_fieldName = fieldName;
	reset(rows, cols);
}

FieldData::FieldData( const FieldData& src )
{
	*(this) = src;
}

bool FieldData::setElement( size_t ithRow, size_t ithCol, const QSharedPointer<AbstractData>& pData )
{
	if (!checkRange(ithRow, ithCol))
		return false;
	size_t idx = getIdx(ithRow, ithCol);
	m_pDataArray[idx] = pData;
	return true;
}

void FieldData::clear()
{
	m_pDataArray.clear();
	m_nRows = m_nCols = 0;
}

FieldData& FieldData::operator=( const FieldData& src )
{
	m_nRows = src.m_nRows;
	m_nCols = src.m_nCols;
	size_t n = m_nRows * m_nCols;
	m_fieldName = src.m_fieldName;

	if (!n)
		return *this;

	m_pDataArray.resize(n);
	for (int i = 0; i < n; ++i)
	{
		const ElementPtr& pSrcData = src.m_pDataArray[i];
		if (pSrcData.isNull())
			continue;

		AbstractData::AbstractType type = pSrcData->getAbstractType();
		ElementPtr& pDst = m_pDataArray[i];
		switch (type)
		{
		case AbstractData::AT_MATRIX:
			pDst = ElementPtr(new MatrixData);
			*((MatrixData*)pDst.data()) = *((MatrixData*)pSrcData.data());
			break;
		case AbstractData::AT_STRUCT:
			pDst = ElementPtr(new StructData);
			*((StructData*)pDst.data()) = *((StructData*)pSrcData.data());
			break;
		}
	}
	return *this;
}

FieldData::ElementPtr FieldData::getElement( size_t ithRow, size_t ithCol )
{
	if (!checkRange(ithRow, ithCol))
		return ElementPtr();
	return m_pDataArray[getIdx(ithRow, ithCol)];
}

#ifdef MATLAB_ENGINE
bool FieldData::sendToArray( mxArray*& structArray )
{
	if (isEmpty())
		return false;

	size_t n = m_nRows * m_nCols;
	QByteArray namebyte = m_fieldName.toAscii();
	const char* fieldNamePtr = namebyte.data();
	bool isSuccess = true;
	for (int i = 0; i < n; ++i)
	{
		ElementPtr pEle = m_pDataArray[i];
		if (pEle.isNull())
			continue;

		mxArray* pMatlabArray = NULL;
		AbstractData::AbstractType type = pEle->getAbstractType();
		if (type != AbstractData::AT_STRUCT &&
			type != AbstractData::AT_MATRIX)
			continue;

		isSuccess &= pEle->sendToArray(pMatlabArray);
		isSuccess &= (pMatlabArray != NULL);
		if(!isSuccess)
			continue;

		int fieldNumber = mxGetFieldNumber(structArray, fieldNamePtr);
		if (fieldNumber == -1)
			continue;
		mxSetFieldByNumber(structArray, i, fieldNumber, pMatlabArray);
	}
	return isSuccess;
}

bool FieldData::getFromArray( mxArray*& structArray )
{
	// clear data
	reset(mxGetM(structArray), mxGetN(structArray));

	size_t n = m_nRows * m_nCols;
	QByteArray nameByte = m_fieldName.toAscii();
	const char* fieldNamePtr = nameByte.data();
	bool isSuccess = true;

	for (size_t i = 0; i < n; ++i)
	{
		mxArray* pMatlabArray = mxGetField(structArray, i, fieldNamePtr);
		isSuccess &= (pMatlabArray != NULL);

		ElementPtr& pEle = m_pDataArray[i]; // all null pointer
		if(mxIsStruct(pMatlabArray))
			pEle = ElementPtr(new StructData);
		else if (MatrixData::isMatrix(pMatlabArray))
			pEle = ElementPtr(new MatrixData);
		pEle->getFromArray(pMatlabArray);
	}
	return true;
}

#endif
void FieldData::reset( size_t rows, size_t cols )
{
	m_pDataArray.clear();
	if (rows <= 0 || cols <= 0)
		rows = cols = 0;
	size_t n = rows * cols;
	m_nRows = rows;
	m_nCols = cols;
	if (n)
	{
		m_pDataArray.resize(n);
	}
}

void FieldData::reset( const QString& name, size_t rows, size_t cols )
{
	setName(name);
	reset(rows, cols);
}

#ifdef MATLAB_ENGINE
bool AbstractData::getFromEngine( Engine* eng, const char* varName )
{ 
	mxArray* matlabArray;
	matlabArray = engGetVariable(eng, varName);
	if (!matlabArray)
		return false;

	clear();
	bool res = getFromArray(matlabArray);
	mxDestroyArray(matlabArray);
	return res;
}

bool AbstractData::sendToEngine( Engine* eng, const char* varName )
{
	if (isEmpty())
		return false;
	mxArray* matlabArray;
	sendToArray(matlabArray);
	engPutVariable(eng, varName, matlabArray);
	mxDestroyArray(matlabArray);
	return true;
}

#endif

bool AbstractData::writeToDataStream( QDataStream &dataStream, const AbstractData* pData )
{
	if (!pData)
		return false;
	AbstractType type = pData->getAbstractType();
	if (type == AbstractData::AT_UNKNOWN)
		return false;
	dataStream << (quint32)type;
	if (type == AbstractData::AT_MATRIX)
	{
		MatrixData& mat = *((MatrixData*)pData);
		dataStream << mat;
	}
	else if (type == AbstractData::AT_STRUCT)
	{
		StructData& str = *((StructData*)pData);
		dataStream << str;
	}
	return true;
}

bool AbstractData::readFromDataStream( QDataStream &dataStream, AbstractData*& pData )
{
	quint32 typeU;
	dataStream >> typeU;
	AbstractType type = (AbstractType)typeU;
	if (type == AbstractData::AT_UNKNOWN)
		return false;
	if (type == AbstractData::AT_MATRIX)
	{
		MatrixData* pMat = new MatrixData;
		dataStream >> *(pMat);
		pData = pMat;
	}
	else if (type == AbstractData::AT_STRUCT)
	{
		StructData* pStr = new StructData;
		dataStream >> *(pStr);
		pData = pStr;
	}
	else
		return false;
	return true;
}

QDataStream& operator>>( QDataStream &dataStream, StructData& s )
{
	s.clear();
	int nFields;
	dataStream >> s.m_rows >> s.m_cols >> nFields;
	for (int ithField = 0; ithField < nFields; ++ithField)
	{
		QString fieldName;
		dataStream >> fieldName;
		FieldData& fieldData = s.m_fields[fieldName];
		dataStream >> fieldData;
	}
	return dataStream;
}

QDataStream& operator<<( QDataStream &dataStream, const StructData& s )
{
	dataStream << s.m_rows << s.m_cols << s.m_fields.size();
	for (QHash<QString, FieldData>::ConstIterator pField = s.m_fields.begin();
		pField != s.m_fields.end(); ++pField)
	{
		dataStream << pField.key();
		dataStream << pField.value();
	}
	return dataStream;
}

QDataStream& operator<<( QDataStream &dataStream, const FieldData& s )
{
	dataStream << s.m_fieldName << s.m_nRows << s.m_nCols;
	for (int i = 0; i < s.m_pDataArray.size(); ++i)
	{
		const FieldData::ElementPtr& pEle = s.m_pDataArray[i];
		bool isNull = pEle.isNull();
		dataStream << isNull;
		if (isNull)
			continue;

		AbstractData::writeToDataStream(dataStream, pEle.data());
	}
	return dataStream;
}

QDataStream& operator>>( QDataStream &dataStream, FieldData& s )
{
	s.clear();
	dataStream >> s.m_fieldName >> s.m_nRows >> s.m_nCols;
	size_t nElements = s.m_nRows * s.m_nCols;
	s.m_pDataArray.resize(nElements);
	for (int i = 0; i < s.m_pDataArray.size(); ++i)
	{
		bool isNull;
		dataStream >> isNull;
		if (isNull)
			continue;

		AbstractData* pEle0;
		AbstractData::readFromDataStream(dataStream, pEle0);
		s.m_pDataArray[i] = FieldData::ElementPtr(pEle0);
	}
	return dataStream;
}
