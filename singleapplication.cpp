#include "singleapplication.h"

#include <QLocalSocket>
#include <QDebug>

SingleApplication::SingleApplication(int &argc, char *argv[], const QString key) :
	QGuiApplication(argc, argv), uniqueKey(key)
{
	sharedMem.setKey(uniqueKey);
	if (sharedMem.attach()){
		running = true;
	}
	else {
		running = false;
		if (!sharedMem.create(1)){
			qDebug() << "An instance is already running.";
			return;
		}
		// create local server to listen for msgs from other instances
		localServer = new QLocalServer(this);
		connect(localServer, SIGNAL(newConnection()), this, SLOT(on_receiveMsg()));
		localServer->listen(uniqueKey);
	}
}

SingleApplication::~SingleApplication()
{
	delete localServer;
}

/* PUBLIC */
// returns whether an instance of the program is already running
bool SingleApplication::isRunning()
{
	return running;
}

// sends a message to the existing instance to activate
bool SingleApplication::sendMsg(const QString &msg)
{
	if (!running){
		return false;
	}
	QLocalSocket localSocket(this);
	localSocket.connectToServer(uniqueKey, QIODevice::WriteOnly);
	if (!localSocket.waitForConnected(timeout)){
		qDebug() << localSocket.errorString().toLatin1();
		return false;
	}
	localSocket.write(msg.toUtf8());
	if (!localSocket.waitForBytesWritten(timeout)){
		qDebug() << localSocket.errorString().toLatin1();
		return false;
	}
	localSocket.disconnectFromServer();
	return true;
}


/* SLOTS */
// recieves a message from a new instance to reactivate this instance of the program
void SingleApplication::on_receiveMsg()
{
	QLocalSocket *localSocket = localServer->nextPendingConnection();
	if (!localSocket->waitForReadyRead(timeout)){
		qDebug()<< localSocket->errorString().toLatin1();
		return;
	}
	QByteArray byteArray = localSocket->readAll();
	QString msg = QString::fromUtf8(byteArray.constData());
	emit msgAvailable(msg);
	localSocket->disconnectFromServer();
}
