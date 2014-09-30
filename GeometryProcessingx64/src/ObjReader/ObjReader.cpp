#include "stdafx.h"
#include "ObjReader.h"
WObjReader::WObjReader()
{
//	readingThread=NULL;
}
WObjReader::~WObjReader()
{
	this->clear();
}
void WObjReader::readObjFile(const char*fileName)
{
	char*flag=new char[50];		//array to receive input		
	bool isPrimitiveBegin=true;	//the first vertex of each primitive
	WObjPrimitive*currPrimitive=NULL;//pointer to current primitive
	WObjTriangle*currTriangle=NULL;//pointer to current triangle
	Wfloat3 threeDCoor;			//3D coordinate
	Wfloat2 twoDCoor;			//2D coordinate
	int currentMtlIndex=-1;		//the index of current Material
	char* pointIndex=new char[20];
	int pointIndexInt;
	string point;
	stringstream pointBuffer;
	bool isGeometryBegin=false;
	unsigned int totalLength;	//文件总长度
	//由于maya和max导出的obj格式有一点
	//不同，maya格式在每个primitive的第一个v之前还会有一个g标记
	//而max没有，这里仅当读到vt标记之后isGeometryBegin才开启，
	//确保仅当读到表示物体名称的g标记才把物体名称记录下来
	int totalVertices=0,totalTexcoords=0,totalNormals=0;
	file.clear();
	file.open((fileName),ios::in);
	if(!file)
	{
		MessageBox(NULL,_T("找不到文件"),_T("错误"),0);
		return;
	}
	file.seekg(0,ios::end);
	totalLength=file.tellg();
	file.seekg(0,ios::beg);
	int checkCounter = 1;
	while(!file.eof())
	{
		checkCounter--;
		if(!checkCounter)
		{
			// 每读OBJ_READ_CHECK_COUNTER行报告一次进度
			checkCounter = OBJ_READ_CHECK_COUNTER;
			readingProcess=float(file.tellg()* 100.0f)/totalLength ;
			cout<<"file reading progress:"<<readingProcess<<"%     \r";
		}
		file>>flag;
	//	cout<<"flag"<<flag<<endl;
		//cout<<flag<<strcmp(flag,"v")<<endl;
		if(!strcmp(flag,"v"))//vertex flag
		{
			//a new primitive must begin with "v"
			//when encounter the first "v" of a primitive 
			//isPrimitiveBegin is true
			if(isPrimitiveBegin)
			{
				if(currPrimitive)
				{
					primitives.push_back(*currPrimitive);
					totalVertices+=currPrimitive->vertices.size();
					totalTexcoords+=currPrimitive->texcoords.size();
					totalNormals+=currPrimitive->normals.size();
					delete currPrimitive;
				}
				currPrimitive=new WObjPrimitive;
				isPrimitiveBegin=false;
			}
			file>>threeDCoor.x
				>>threeDCoor.y
				>>threeDCoor.z;//vertex coordinate
			currPrimitive->vertices.push_back(threeDCoor);
			//cout<<threeDCoor.x<<threeDCoor.y<<threeDCoor.z<<endl;
		}
		else
		{
			isPrimitiveBegin=true;
			if(!strcmp(flag,"vt"))//vertex texcoord flag
			{
				isGeometryBegin=true;
				file>>twoDCoor.u
					>>twoDCoor.v;
				currPrimitive->texcoords.push_back(twoDCoor);
				//cout<<twoDCoor.u<<twoDCoor.v<<endl;
			}
			else if(!strcmp(flag,"vn"))//vertex normal flag
			{
				file>>threeDCoor.x
					>>threeDCoor.y
					>>threeDCoor.z;
				currPrimitive->normals.push_back(threeDCoor);
				//cout<<threeDCoor.x<<threeDCoor.y<<threeDCoor.z<<endl;
			}
			else if(!strcmp(flag,"g"))//primitive name flag
			{
				file>>flag;string a;
				if(isGeometryBegin)
				{
					currPrimitive->name=flag;
					isGeometryBegin=false;
				}
				//cout<<currPrimitive->name<<endl;
			}
			else if(!strcmp(flag,"usemtl"))//material flag
			{				
				string mtlName;
				vector<WObjMaterial>::iterator mtlIterator;
				WObjMaterial*newMaterial;
				currentMtlIndex=-1;
				//update current material
				file>>mtlName;
				for(mtlIterator=Materials.begin();
					mtlIterator!=Materials.end();
					mtlIterator++)
					//find if there is an existing material with the 
					//same name
				{
					currentMtlIndex++;
					if(mtlIterator->name==mtlName)
						break;//find
				}
				if(mtlIterator==Materials.end())
					//can't find,so create a new material
				{
					newMaterial=new WObjMaterial;
					newMaterial->name=mtlName;
					Materials.push_back(*newMaterial);
					delete newMaterial;
					currentMtlIndex=Materials.size()-1;
				}
				//cout<<Materials[0].name<<endl;
			}
			else if (!strcmp(flag,"f"))//face flag
			{
				currTriangle=new WObjTriangle;
				for(int i=0;i<3;i++)
				{
					//a triangle vertex with 3 indices
					//for example point="9/10/11"
					file>>point;
					pointBuffer.clear();
					pointBuffer<<point;
					//now ,pointBuffer="9/10/11"
					//fill vertex index
					pointBuffer.getline(pointIndex,20,'/');
					pointIndexInt=atoi(pointIndex);
					currTriangle->vertIndex[i]=(unsigned int)(
						pointIndexInt>0?
						pointIndexInt-1-totalVertices:
						currPrimitive->vertices.size()+pointIndexInt);
					//fill vertex texcoord index
					pointBuffer.getline(pointIndex,20,'/');
					pointIndexInt=atoi(pointIndex);
					currTriangle->texcoordIndex[i]=(unsigned int)(
						pointIndexInt>0?
						pointIndexInt-1-totalTexcoords:
					currPrimitive->texcoords.size()+pointIndexInt);
					//fill vertex normal index
					pointBuffer.getline(pointIndex,20,'/');
					pointIndexInt=atoi(pointIndex);
					currTriangle->norIndex[i]=(unsigned int)(
						pointIndexInt>0?
						pointIndexInt-1-totalNormals:
					currPrimitive->normals.size()+pointIndexInt);

				}
				currTriangle->mtlIndex=currentMtlIndex;
				currPrimitive->faces.push_back(*currTriangle);
				delete currTriangle;
			}
		}
	}
	primitives.push_back(*currPrimitive);
	file.close();
	file.clear();
	//读完后文件状态字会设置成endofbit，clear函数恢复状态字为goodbit
	delete[] flag;
	delete[] pointIndex;
	cout<<"complete to read obj file            \n"<<endl;
	cout<<endl;
}
void WObjReader::showObj()
{
	vector<WObjPrimitive>::iterator itPrimitive;
	vector<Wfloat3>::iterator itFloat3;
	vector<Wfloat2>::iterator itFloat2;
	vector<WObjTriangle>::iterator itTriangle;

	for(itPrimitive=primitives.begin();
		itPrimitive!=primitives.end();
		itPrimitive++)
	{
		cout<<"\n##################"<<endl;
		cout<<"Object Name:"+itPrimitive->name<<endl;
		cout<<"##################"<<endl;
		cout<<"\nvertices:"<<endl;
		for(itFloat3=itPrimitive->vertices.begin();
			itFloat3!=itPrimitive->vertices.end();
			itFloat3++)
		{
			cout<<itFloat3->x<<'\t'
				<<itFloat3->y<<'\t'
				<<itFloat3->z<<'\t'<<endl;
		}
		cout<<"\nnormals:"<<endl;
		for(itFloat3=itPrimitive->normals.begin();
			itFloat3!=itPrimitive->normals.end();
			itFloat3++)
		{
			cout<<itFloat3->x<<'\t'
				<<itFloat3->y<<'\t'
				<<itFloat3->z<<'\t'<<endl;
		}
		cout<<"\ntexcoords:"<<endl;
		for(itFloat2=itPrimitive->texcoords.begin();
			itFloat2!=itPrimitive->texcoords.end();
			itFloat2++)
		{
			cout<<itFloat2->u<<'\t'
				<<itFloat2->v<<'\t'<<endl;
		}
		cout<<"\nfaces:"<<endl;
		for(itTriangle=itPrimitive->faces.begin();
			itTriangle!=itPrimitive->faces.end();
			itTriangle++)
		{
			for(int i=0;i<3;i++)
			{
				cout<<itTriangle->vertIndex[i]<<'/'
					<<itTriangle->texcoordIndex[i]<<'/'
					<<itTriangle->norIndex[i]<<'\t';
			}
			cout<<"materialID:"<<itTriangle->mtlIndex<<"  material:"
				<<Materials[itTriangle->mtlIndex].name<<endl;
		}

	}
}
void WObjReader::clear()
{
	vector<WObjPrimitive>::iterator pP;
	for(pP=primitives.begin();pP<primitives.end();pP++)
	{
		pP->faces.clear();
		pP->normals.clear();
		pP->texcoords.clear();
		pP->vertices.clear();
	}
	primitives.clear();
	Materials.clear();
}

void WObjReader::getCoordArray(float*&pArray,unsigned int&nFloats,unsigned nthPrimitive,coordType type)
{
	unsigned int num;
	switch(type)
	{
	case VERTCOORDS:
		num=primitives[nthPrimitive].vertices.size();//点数
		nFloats=num*3;//坐标数
		pArray=new float[nFloats];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[3*i]=primitives[nthPrimitive].vertices[i].x;
			pArray[3*i+1]=primitives[nthPrimitive].vertices[i].y;
			pArray[3*i+2]=primitives[nthPrimitive].vertices[i].z;
		}
		break;
	case TEXCOORDS:
		num=primitives[nthPrimitive].texcoords.size();//贴图坐标数
		nFloats=num*2;//坐标数
		pArray=new float[nFloats];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[2*i]=primitives[nthPrimitive].texcoords[i].u;
			pArray[2*i+1]=primitives[nthPrimitive].texcoords[i].v;
		}
		break;
	case NORCOORDS:
		num=primitives[nthPrimitive].normals.size();//法向量数
		nFloats=num*3;//坐标数
		pArray=new float[nFloats];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[3*i]=primitives[nthPrimitive].normals[i].x;
			pArray[3*i+1]=primitives[nthPrimitive].normals[i].y;
			pArray[3*i+2]=primitives[nthPrimitive].normals[i].z;
		}
		break;
	default:
		nFloats=0;
		pArray=NULL;
	}
	return;
}
void WObjReader::getIndexArray(unsigned int*&pArray,unsigned int &nInts,
				   unsigned nthPrimitive,indexType type)
{
	unsigned int num;
	switch(type)
	{
	case VERTINDICES:
		num=primitives[nthPrimitive].faces.size();//面数
		nInts=num*3;//索引数
		pArray=new unsigned int[nInts];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[i*3]=primitives[nthPrimitive].faces[i].vertIndex[0];//第一个顶点的索引
			pArray[i*3+1]=primitives[nthPrimitive].faces[i].vertIndex[1];//第二个顶点的索引
			pArray[i*3+2]=primitives[nthPrimitive].faces[i].vertIndex[2];//第三个顶点的索引
		}
		break;
	case TEXINDICES:
		num=primitives[nthPrimitive].faces.size();//面数
		nInts=num*3;//索引数
		pArray=new unsigned int[nInts];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[i*3]=primitives[nthPrimitive].faces[i].texcoordIndex[0];//第一个顶点的索引
			pArray[i*3+1]=primitives[nthPrimitive].faces[i].texcoordIndex[1];//第二个顶点的索引
			pArray[i*3+2]=primitives[nthPrimitive].faces[i].texcoordIndex[2];//第三个顶点的索引
		}
		break;
	case NORINDICES:
		num=primitives[nthPrimitive].faces.size();//面数
		nInts=num*3;//索引数
		pArray=new unsigned int[nInts];
		for(unsigned int i=0;i<num;i++)
		{
			pArray[i*3]=primitives[nthPrimitive].faces[i].norIndex[0];//第一个顶点的索引
			pArray[i*3+1]=primitives[nthPrimitive].faces[i].norIndex[1];//第二个顶点的索引
			pArray[i*3+2]=primitives[nthPrimitive].faces[i].norIndex[2];//第三个顶点的索引
		}
		break;
	}
	return;
}

void WObjReader::readMtlFile(const char*fileName)
{
	char flag[50];

	file.open(fileName,ios::in);
	if(!file)
	{
		cout<<"error occurs when open mtl file "<<endl;
		return;
	}
	while(!file.eof())
	{
		file>>flag;

		if(!strcmp(flag,"newmtl"))
		{
			string mtlName;	
			file>>mtlName;
			WObjMaterial newMtl;
			newMtl.name=mtlName;
			newMtl.emission.x = newMtl.emission.y = newMtl.emission.z = 0;
			newMtl.specular = 0;
			Materials.push_back(newMtl);
		}
		//漫反射颜色对应的flag
		if(!strcmp(flag,"Kd"))
		{
			WObjMaterial& newMtl = Materials.back();
			file>>newMtl.diffuse.x
				>>newMtl.diffuse.y
				>>newMtl.diffuse.z;
		}
		//自发光颜色对应的flag(借用了环境光通道)
		if(!strcmp(flag,"Ke"))
		{
			WObjMaterial& newMtl = Materials.back();
			float x,y,z;
			file >> x >> y >> z;
			newMtl.emission.x = x * 5;
			newMtl.emission.y = y * 5;
			newMtl.emission.z = z * 5;
		}
		else if(!strcmp(flag,"Tf"))
		{
			WObjMaterial& newMtl = Materials.back();
			float x,y,z;
			file >> x >> y >> z;
			newMtl.transparency.x = x;
			newMtl.transparency.y = y;
			newMtl.transparency.z = z;
		}
		else if(!strcmp(flag,"Ks"))
		{
			WObjMaterial& newMtl = Materials.back();
			float x,y,z;
			file >> x >> y >> z;
			newMtl.specular = (x+y+z)/3;
		}
	}
	file.close();	file.clear();	
	cout<<"complete to read mtl file "<<endl;
	//读完后文件状态字会设置成endofbit，clear函数恢复状态字为goodbit
}

void WObjReader::showMtl()
{
	vector<WObjMaterial>::iterator p;
	for(p=Materials.begin();p!=Materials.end();p++)
	{
		cout<<"\n##################"<<endl;
		cout<<"Material Name:"+p->name<<endl;
		cout<<"##################"<<endl;
		cout<<"diffuse color: "
			<<p->diffuse.x<<'\t'
			<<p->diffuse.y<<'\t'
			<<p->diffuse.z<<endl;
	}
}
