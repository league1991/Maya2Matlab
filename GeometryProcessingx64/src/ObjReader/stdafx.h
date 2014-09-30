// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include <stdlib.h>
#include <vector>
#include <map>/*
#include <ostream>
#include <istream>
#include <fstream>
#include <sstream>*/
#include <matrix.h>
#include <engine.h>
#include <tmwtypes.h>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#include <Eigen/Dense>
#include <eigen/sparse>
#include <Eigen/LU>
#include <Eigen/SVD>
using Eigen::Affine;
using namespace Eigen;

#include <QApplication>
#include <QString>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QSharedPointer>
#include <QThread>
#include <QDir>

#include <engine.h>
#include <matrix.h>

#define DATA_STREAM_VERSION    QDataStream::Qt_4_0

#include "MatrixData.h"
#include "ExePacket.h"
#include "ExeServer.h"
#include "ObjReader.h"
#include "task.h"




