#pragma once

class ReadObjTask:public ExecTask
{
public:
	ReadObjTask(const QString& name):ExecTask(name)
	{
		m_path = "H:/3D/GPscenes/cloth detection/meshData/";
		addOutputSlot("frameMatrix");
	}

	void getVertArray(vector<vector<Wfloat3>>& vtx);
	bool compute();

private:
	QString m_path;
};

class WritePlyTask:public ExecTask
{
public:
	WritePlyTask(const QString& name):ExecTask(name)
	{
		m_path = "H:/3D/GPscenes/cloth detection/meshData/";
		this->addInputSlot("vertex");
		this->addInputSlot("face");
		this->addInputSlot("frame");
	}
	
	bool compute();
private:

	//Write Mesh to .ply file
	//vertexMatrix				#vertices * 3		Position per vertex	float
	//vertexNormalMatrix	#vertices * 3		Normal	 per	vertex	float		pass nullptr if not existed
	//vertexColorMatrix		#vertices * 3		Color	 per	vertex	int		pass nullptr if not existed
	//faceMatrix					#faces	* #vertices per face				int
	bool writeToPLY(Eigen::MatrixXd *vertexMatrix, 
					Eigen::MatrixXf *vertexNormalMatrix, 
					Eigen::MatrixXi *vertexColorMatrix, 
					Eigen::SparseMatrix<int> *faceMatrix, 
					const char* fileName);

	QString m_path;
};
// 
// class FittingExe:public ExecTask
// {
// public:
// 	FittingExe(const QString& name)
// 	{
// 		addInputSlot("dataPoint");
// 		addInputSlot("curvePoint");
// 		addInputSlot("offset");
// 		addOutputSlot("A");
// 		addOutputSlot("B");
// 		addOutputSlot("D");
// 	}
// 
// 	bool compute();
// };
// 
