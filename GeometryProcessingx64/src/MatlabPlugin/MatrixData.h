#pragma once
#include "engine.h"

class AbstractData
{
public:
	typedef QSharedPointer<AbstractData> AbstractDataPtr;
	// VERY IMPORTANT!
	// virtual destructor, to ensure child class releasing memory
	virtual ~AbstractData(){};
	enum AbstractType{AT_UNKNOWN, AT_STRUCT, AT_MATRIX};
	inline AbstractType getAbstractType()const{return m_absType;}

	virtual void        clear()=0;
	virtual bool		isEmpty()const=0;		// row <= 0 || col <= 0

#ifdef MATLAB_ENGINE
	// send or get data to/from mxArray, you should call mxDestroyArray(matlabArray) after these functions
	virtual bool		sendToArray(mxArray*& matlabArray)=0;
	virtual bool		getFromArray(mxArray* matlabArray)=0;
	// send or get data to/from matlab engine, you should call mxDestroyArray(matlabArray) after these functions
	virtual bool		sendToEngine(Engine* eng, const char* varName);
	virtual bool		getFromEngine(Engine* eng, const char* varName);
#endif

	static bool         writeToDataStream(QDataStream &dataStream, const AbstractData* pData);
	static bool         readFromDataStream(QDataStream &dataStream, AbstractData*& pData);
protected:
	AbstractType m_absType;
};

class MatrixData:public AbstractData
{
public:
	enum MatrixType{
		MT_UNKNOWN = 0,
		MT_DENSE   = 1,
		MT_SPARSE  = 1<<1,
		MT_ALL     = MT_DENSE|MT_SPARSE
	};
	// enum indicates the type of internal data
	enum DataType{
		DT_UNKNOWN = 0,
		DT_FLOAT   = 1,	
		DT_DOUBLE  = 1<<1,
		DT_INT8    = 1<<2,
		DT_INT32   = 1<<3,
		DT_UINT8   = 1<<4,
		DT_UINT32  = 1<<5,
		DT_BOOL    = 1<<6,
		DT_ALL     = DT_FLOAT|DT_DOUBLE|DT_INT8|DT_INT32|DT_UINT8|DT_UINT32|DT_BOOL
	};

	MatrixData();
	MatrixData(const MatrixData& src);
	~MatrixData();
	bool isEmpty()const;
	void clear();

	static int  getByteCount(DataType type);
	inline DataType    getDataType(){return m_dataType;}
	inline MatrixType  getMatrixType(){return m_matType;}

	// set data of dense/sparse matrix, copy occurs
	void setDenseData( const void* const data, size_t nRows, size_t nCols, DataType type);
	void setSparseData(const void* const data, mwIndex* rowID, mwIndex* rowRange, 
		size_t nRows, size_t nCols, size_t nRowID, size_t nColRange, DataType type);

	// set data of dense/sparse matrix, no copy
	void setDenseDataPtr( void* const data, size_t nRows, size_t nCols, DataType type);
	void setSparseDataPtr(void* const data, mwIndex* rowID, mwIndex* rowRange, 
		size_t nRows, size_t nCols, size_t nRowID, size_t nColRange, DataType type);

	// get data of dense/sparse matrix, no copy
	inline void getDenseDataPtr(void* &data, size_t& nRows, size_t& nCols, DataType& type)
	{
		data = m_voidData; nRows = m_nRows; nCols = m_nCols;type = m_dataType;
	}
	inline void getSparseDataPtr(void*& data, mwIndex*& rowID, mwIndex*& rowRange, 
		size_t& nRows, size_t& nCols, size_t& nRowID, size_t& nColRange, DataType& type)
	{
		data = m_voidData; nRows = m_nRows; nCols = m_nCols;type = m_dataType;
		rowID = m_rowID; rowRange = m_colRange; nRowID = m_nRowIDs; nColRange = m_nColRanges;
	}
	// get data of dense matrix, copy and cast to double,
	// should allocate data externally
	void getDenseDataDouble(double* data);
	void getDenseDataInt(int* data);

	inline void getDimension(size_t& nRows, size_t& nCols){nRows = m_nRows; nCols = m_nCols;}
	inline size_t  getRows(){return m_nRows;}
	inline size_t  getCols(){return m_nCols;}
	MatrixData& operator=(const MatrixData& s);

#ifdef EIGEN
	// Convert MatrixData To Eigen Dense Matrix
	bool toEigenIntDenseMatrix(Eigen::MatrixXi &matrix);
	bool toEigenFloatDenseMatrix(Eigen::MatrixXf &matrix);
	bool toEigenDoubleDenseMatrix(Eigen::MatrixXd &matrix);

	// Convert MatrixData To Eigen Sparse Matrix
	bool toEigenIntSparseMatrix(Eigen::SparseMatrix<int> &matrix);
	bool toEigenFloatSparseMatrix(Eigen::SparseMatrix<float> &matrix);
	bool toEigenDoubleSparseMatrix(Eigen::SparseMatrix<double> &matrix);
#endif

	friend QDataStream& operator<<(QDataStream &dataStream, const MatrixData& mat);
	friend QDataStream& operator>>(QDataStream &dataStream, MatrixData& mat);

#ifdef MATLAB_ENGINE
	bool   sendToArray(mxArray*& matlabArray);
	bool   getFromArray(mxArray* matlabArray);
	static mxClassID    convertNativeToMatlabType(DataType type);
	static DataType		convertMatlabToNativeType(mxClassID matType);
	static bool         isMatrix(mxArray* matlabArray);
#endif
private:
	union										// raw matrix data
	{
		void*            m_voidData;
		float*           m_floatData;
		double*          m_doubleData;
	};

	mwIndex*				 m_rowID;				// sparse data, indices of non-zero elements of each row
	mwIndex*				 m_colRange;			// sparse data, rowID range of each row

	size_t               m_nRows, m_nCols;		// dimension of the matrix
	size_t				 m_nRowIDs,m_nColRanges;// demensions of rowID & colRange
	MatrixType			 m_matType;				// matrixType (dense/sparse)
	DataType			 m_dataType;			// dataType of matrix
};												// data of matrix

class FieldData
{
public:
	typedef AbstractData::AbstractDataPtr ElementPtr;
	FieldData(const QString& fieldName = "", size_t rows = 0, size_t cols = 0);
	FieldData(const FieldData& src);
	~FieldData(){clear();}

	ElementPtr		getElement(size_t ithRow, size_t ithCol);
	bool			setElement(size_t ithRow, size_t ithCol, const ElementPtr& pData);

	void			clear();
	void            setName(const QString& name){m_fieldName = name;}
	void            reset(size_t rows, size_t cols);			// clear all data and then resize 
	void            reset(const QString& name,size_t rows, size_t cols);
	inline size_t      rows()const{return m_nRows;}
	inline size_t      cols()const{return m_nCols;}
	inline bool     isEmpty()const{return m_nRows <= 0 || m_nCols <= 0;}
	// deep copy
	FieldData&		operator=(const FieldData& src);


#ifdef MATLAB_ENGINE
	// send data to matlab array
	bool			sendToArray(mxArray*& structArray);
	// get data from matlab array,
	// all the field data should be cleared in advance
	bool			getFromArray(mxArray*& structArray );
#endif

	friend QDataStream& operator<<(QDataStream &dataStream, const FieldData& s);
	friend QDataStream& operator>>(QDataStream &dataStream, FieldData& s);

private:
	inline bool		checkRange(size_t r, size_t c)
	{return r >= 0 && r < m_nRows && c >= 0 && c < m_nCols;}

	inline size_t		getIdx(int row, int col)
	{return m_nRows * col + row;}

	QString								m_fieldName;
	size_t									m_nRows, m_nCols;

	QVector<ElementPtr>					m_pDataArray;		// data array, stored by column-major order
};

// matlab struct data
class StructData:public AbstractData
{
public:
	StructData();
	StructData(const StructData& src);
	~StructData();

	// recursive call
	StructData& operator=(const StructData& d);
	void clear();

	bool			isEmpty()const{return m_rows <= 0 || m_cols <= 0;}
	bool			addField(const QString& fieldName, const FieldData& fieldData);
	FieldData*		getField(const QString& fieldName);

	bool					addElement(const QString& fieldName, int row, int col, FieldData::ElementPtr pData);
	FieldData::ElementPtr	getElement(const QString& fieldName, int row, int col);
	
#ifdef MATLAB_ENGINE
	bool			sendToArray(mxArray*& structArray);
	bool			getFromArray(mxArray* structArray );
#endif

	friend QDataStream& operator<<(QDataStream &dataStream, const StructData& s);
	friend QDataStream& operator>>(QDataStream &dataStream, StructData& s);
private:
	void            allocateFieldNameArray(char**& nameArray, int& length);
	void            releaseFieldNameArray(char**& nameArray, int length);

	size_t                        m_rows, m_cols;
	QHash<QString, FieldData>  m_fields;
};