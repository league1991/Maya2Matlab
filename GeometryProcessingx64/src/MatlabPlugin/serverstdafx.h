
#include <stdlib.h>

#include <exception>

#include <QApplication>
#include <QString>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QSharedPointer>
#include <QThread>

#define DATA_STREAM_VERSION    QDataStream::Qt_4_0

#include "MatrixData.h"
#include "ExePacket.h"
#include "ExeServer.h"

#include <engine.h>
#include <matrix.h>



