#pragma once
/************************************************************************
Custom Data Type:		matlabMatrix
setAttr command format: setAttr (flag) (nRows) (nCols) (data)
flags:
-dense	-d				specify a dense matrix
/************************************************************************/

class MatlabData:public MPxData
{
public:
	enum Type
	{
		MATLAB_UNKNOWN,			// matlab data not supported yet, for example: cell array
		MATLAB_MATRIX,			// matlab matrix data
		MATLAB_STRUCT			// matlab struct data
	};
	virtual ~MatlabData();
	//This method initializes an instance of an MPxData derived class from another existing instance. 
	virtual void 			copy (const MPxData &src)=0;
	// Determines the type id of the Data object. 
	virtual MTypeId 		typeId () const=0;
	virtual MString 		name () const=0;
	virtual void			clear()=0;

	virtual bool            sendDataToMatlab(Engine* eng, const MString& varName)=0;
	virtual bool            getDataFromMatlab( Engine* eng, const MString& varName )=0;

	static Type             getDataType(MObject&matObj);
	static MStatus          checkAndGetData( MObject&matObj, MatlabData*& dat);
	static Type             getDataType(Engine* eng, const char* varName);
protected:
};
class MatlabMatrix: public MatlabData
{
public:

	MatlabMatrix(void);
	~MatlabMatrix(void);

	static void*	creator();

	// These four methods do NOT work now!  
/*
	//Creates Data in Data Block as specified by input from ASCII file record.  */
	virtual MStatus		readASCII (const MArgList &argList, unsigned int &endOfTheLastParsedElement);
	// Encodes Data in accordance with the ASCII file format and outputs it to the given stream.
	virtual MStatus		writeASCII( std::ostream& out) ;
	//Creates Data in Data Block as specified by binary data from the given stream. 
	virtual MStatus		readBinary (std::istream &in, unsigned int length);
	//Encodes Data in accordance with the binary file format and outputs it to the given stream. 
	virtual MStatus		writeBinary( std::ostream& out);

	//This method initializes an instance of an MPxData derived class from another existing instance. 
	void 			copy (const MPxData &src);
	// Determines the type id of the Data object. 
	MTypeId 		typeId () const{return m_id;}
	static MTypeId  getTypeId() {return m_id;}
	MString 		name () const{return m_name;}

	// get the matrix object, then you can set its data
	MatrixData&     getMatrix(){return m_D;}

	void			clear();

	bool            sendDataToMatlab(Engine* eng, const MString& varName);
	bool            getDataFromMatlab( Engine* eng, const MString& varName );

	// create maya object of MatlabMatrix and retrieve its data
	// matPtr points to the data held in obj
	static MStatus  createMatrixObject(MObject& obj, MatlabMatrix*& matPtr);

	// helper function. Helps to get and validate matrix data from maya.
	// don't delete mat because it's actually managed by matObj
	static MStatus  checkAndGetMatrixPtr(	MObject&matObj, MatlabMatrix*& mat, 
										MatrixData::MatrixType matType = MatrixData::MT_ALL, 
										MatrixData::DataType dataType = MatrixData::DT_ALL);
private:
	void			clearData();

	// internal matrix data, store as column order, which is the same as MATLAB
	MatrixData			 m_D;

	// maya properties
	static const MTypeId m_id;
	static const char*   m_name;
};


class MatlabStruct:public MatlabData
{
public:

	MatlabStruct(void){}
	~MatlabStruct(void);

	static void*	creator();

	// These four methods do NOT work now!  
/*
	//Creates Data in Data Block as specified by input from ASCII file record.  */
	virtual MStatus		readASCII (const MArgList &argList, unsigned int &endOfTheLastParsedElement){return MStatus::kSuccess;}
	// Encodes Data in accordance with the ASCII file format and outputs it to the given stream.
	virtual MStatus		writeASCII( std::ostream& out){return MStatus::kSuccess;}
	//Creates Data in Data Block as specified by binary data from the given stream. 
	virtual MStatus		readBinary (std::istream &in, unsigned int length){return MStatus::kSuccess;}
	//Encodes Data in accordance with the binary file format and outputs it to the given stream. 
	virtual MStatus		writeBinary( std::ostream& out){return MStatus::kSuccess;}

	//This method initializes an instance of an MPxData derived class from another existing instance. 
	void 			    copy (const MPxData &src);

	// Determines the type id of the Data object. 
	MTypeId 		typeId () const{return m_id;}
	static MTypeId  getTypeId() {return m_id;}
	MString 		name () const{return m_name;}

	// get the matrix object, then you can set its data
	StructData&     getStruct(){return m_D;}

	void			clear(){m_D.clear();}

	bool            sendDataToMatlab(Engine* eng, const MString& varName);
	bool            getDataFromMatlab( Engine* eng, const MString& varName );

	// create maya object of MatlabMatrix and retrieve its data
	// matPtr points to the data held in obj
	static MStatus  createStructObject(MObject& obj, MatlabStruct*& matPtr);

	// helper function. Helps to get and validate matrix data from maya.
	// don't delete mat because it's actually managed by matObj
	static MStatus  checkAndGetStruct(	MObject&matObj, MatlabStruct*& mat);

private:
	void			clearData();

	// internal matrix data, store as column order, which is the same as MATLAB
	StructData			 m_D;

	// maya properties
	static const MTypeId m_id;
	static const char*   m_name;
};
