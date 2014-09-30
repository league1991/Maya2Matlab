#include "stdafx.h"
using namespace Eigen;
void ReadObjTask::getVertArray( vector<vector<Wfloat3>>& vtx )
{
	QDir dir(m_path);
	QStringList fileList = dir.entryList(QDir::Files, QDir::Name);
	int nFiles = fileList.size();

	for (int ithFile = 0; ithFile < nFiles; ++ithFile)
	{
		QString fileName = m_path + fileList[ithFile];
		QByteArray fileNameB = fileName.toAscii();

		WObjReader reader;
		reader.readObjFile(fileNameB.data());
		const vector<WObjPrimitive>& pris = reader.getPrimitives();
		if (pris.size() == 0)
			continue;

		vtx.push_back(pris[0].vertices);
	}
}

bool ReadObjTask::compute()
{
	vector<vector<Wfloat3>> vtx;
	getVertArray(vtx);
	int nVtx = vtx[0].size();
	int nFrames = vtx.size();
	int totElements = nVtx * (nFrames * 3);
	double* data = new double[totElements];
	for (int ithFrame = 0; ithFrame < nFrames; ++ithFrame)
	{
		double* vertX = data  + nVtx * ithFrame * 3;
		double* vertY = vertX + nVtx;
		double* vertZ = vertY + nVtx;

		vector<Wfloat3>& frameVert = vtx[ithFrame];
		if (frameVert.size() != nVtx)
			continue;

		for (int ithVert = 0; ithVert < nVtx; ++ithVert)
		{
			Wfloat3& f3 = frameVert[ithVert];
			vertX[ithVert] = f3.x;
			vertY[ithVert] = f3.y;
			vertZ[ithVert] = f3.z;
		}
	}

	MatrixData* out0 = getOutputData("frameMatrix");
	if (!out0)
		delete[] data;
	else
		out0->setDenseDataPtr(data, nVtx, nFrames*3, MatrixData::DT_DOUBLE);

	m_infoBuffer = "computation finished!\n";
	return true;
}


bool WritePlyTask::writeToPLY(
				Eigen::MatrixXd *vertexMatrix, 
				Eigen::MatrixXf *vertexNormalMatrix, 
				Eigen::MatrixXi *vertexColorMatrix, 
				Eigen::SparseMatrix<int> *faceMatrix, 
				const char* path)
{

	if (vertexMatrix==NULL || faceMatrix == NULL)
	{
		std::cout<<"Vertex Position and Face Indices Needed"<<std::endl;
		return false;
	}

	std::ofstream fout;
	fout.open(path);
	if (!fout)
	{
		std::cout<<"file open failed"<<std::endl;
		return false;
	}

	int vertexNum = vertexMatrix->rows();
	int vertexDim = vertexMatrix->cols();
	int vertexColorDim = (vertexColorMatrix!=NULL)? vertexColorMatrix->cols():0;
	int vertexNormalDim = (vertexNormalMatrix!=NULL)?vertexNormalMatrix->cols():0;
	int faceNum = faceMatrix->cols();



	//Build Header
	fout<<"ply"<<std::endl;
	fout<<"format ascii 1.0"<<std::endl;
	fout<<"element vertex "<<vertexNum<<std::endl;
	fout<<"property float x"<<std::endl;
	fout<<"property float y"<<std::endl;
	fout<<"property float z"<<std::endl;
	if (vertexNormalDim)
	{
		fout<<"property float nx"<<std::endl;
		fout<<"property float ny"<<std::endl;
		fout<<"property float nz"<<std::endl;
	}
	if (vertexColorDim)
	{
		fout<<"property uchar red"<<std::endl;
		fout<<"property uchar green"<<std::endl;
		fout<<"property uchar blue"<<std::endl;
		if(vertexColorDim==4)
			fout<<"property uchar alpha"<<std::endl;
	}
	fout<<"element face "<<faceNum<<std::endl;
	fout<<"property list uchar int vertex_indices"<<std::endl;
	fout<<"end_header"<<std::endl;

	//Record Vertex Coordinates
	for (int i = 0; i < vertexNum; i++)
	{
		fout<<vertexMatrix->row(i)<<" ";
		if (vertexNormalDim)
		{
			fout<<vertexNormalMatrix->row(i)<<" ";
		}
		if (vertexColorDim)
		{
			fout<<vertexColorMatrix->row(i)<<" ";
		}
		fout<<std::endl;
	}

	//Record Face Indices
	for (int i= 0; i< faceNum; i++)
	{
		int faceDim = 0;
		for (Eigen::SparseMatrix<int>::InnerIterator it(*faceMatrix, i); it; ++it)
			++faceDim;

		fout<<faceDim<<" ";

		vector<int> faceMap;
		faceMap.resize(faceDim);
		for (Eigen::SparseMatrix<int>::InnerIterator it(*faceMatrix, i); it; ++it)
		{
			if(it.value())
				faceMap[it.value()-1] = it.row();
		}

		for (int ithIdx = 0; ithIdx < faceDim; ++ithIdx)
			fout<<faceMap[ithIdx] << " ";
		fout<<std::endl;
	}
	return true;

}

bool WritePlyTask::compute()
{
	MatrixData* vert = getInputData("vertex");
	MatrixData* face = getInputData("face");
	MatrixData* frame= getInputData("frame");

	if (!vert || !face)
		return false;

	Eigen::MatrixXd vertF, frameF;
	Eigen::SparseMatrix<int> faceF;
	vert->toEigenDoubleDenseMatrix(vertF);
	face->toEigenIntSparseMatrix(faceF);
	frame->toEigenDoubleDenseMatrix(frameF);
	int ithFrame = (int)frameF(0,0);
	QString frameStr = QString("H:/3D/GPscenes/cloth detection/frameData/frame%1.ply").arg(ithFrame);
	writeToPLY(&vertF, NULL, NULL, &faceF, frameStr.toAscii());
	return true;
}
// 
// bool FittingExe::compute()
// {
// 	MatrixData* dataPnt = getInputData("dataPoint");
// 	MatrixData* curvePnt= getInputData("curvePnt");
// 	MatrixData* offset  = getInputData("offset");
// 
// 	if (!dataPnt || !curvePnt || !offset)
// 		return false;
// 
// 	MatrixXd dataPntE, curvePntE, offsetE;
// 	dataPnt->toEigenDoubleDenseMatrix(dataPnt);
// 	curvePnt->toEigenDoubleDenseMatrix(curvePntE);
// 	offset->toEigenDoubleDenseMatrix(offsetE);
// 
// 
// }
