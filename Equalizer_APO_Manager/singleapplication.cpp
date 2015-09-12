#include "singleapplication.h"
#include <QLocalSocket>
#include <QDebug>

SingleApplication::SingleApplication(int &argc, char *argv[], const QString key) :
	QApplication(argc, argv), uniqueKey(key)
{
	sharedMem.setKey(uniqueKey);
	if (sharedMem.attach())
	{
		_isRunning = true;
	}
	else
	{
		_isRunning = false;
		if (!sharedMem.create(1))
		{
			qDebug() << "An instance is already running.";
			return;
		}
		// create local server to listen for msgs from other instances
		localServer = new QLocalServer(this);
		connect(localServer, SIGNAL(newConnection()), this, SLOT(receiveMsg()));
		localServer->listen(uniqueKey);
	}
}

SingleApplication::~SingleApplication()
{
	delete localServer;
}

// public slots
void SingleApplication::receiveMsg()
{
	QLocalSocket *localSocket = localServer->nextPendingConnection();
	if (!localSocket->waitForReadyRead(timeout))
	{
		qDebug()<< localSocket->errorString().toLatin1();
		return;
	}
	QByteArray byteArray = localSocket->readAll();
	QString msg = QString::fromUtf8(byteArray.constData());
	emit msgAvailable(msg);
	localSocket->disconnectFromServer();
}

// public functions
bool SingleApplication::isRunning()
{
	return _isRunning;
}

bool SingleApplication::sendMsg(const QString &msg)
{
	if (!_isRunning)
	{
		return false;
	}
	QLocalSocket localSocket(this);
	localSocket.connectToServer(uniqueKey, QIODevice::WriteOnly);
	if (!localSocket.waitForConnected(timeout))
	{
		qDebug() << localSocket.errorString().toLatin1();
		return false;
	}
	localSocket.write(msg.toUtf8());
	if (!localSocket.waitForBytesWritten(timeout))
	{
		qDebug() << localSocket.errorString().toLatin1();
		return false;
	}
	localSocket.disconnectFromServer();
	return true;
}
