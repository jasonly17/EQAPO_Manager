#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QGuiApplication>
#include <QSharedMemory>
#include <QLocalServer>

class SingleApplication : public QGuiApplication
{
	Q_OBJECT
public:
	SingleApplication(int &argc, char *argv[], const QString key);
	~SingleApplication();
	bool isRunning();
	bool sendMsg(const QString &message);

public slots:
	void on_receiveMsg();

signals:
	void msgAvailable(QString msg);

private:
	bool running;
	QString uniqueKey;
	QSharedMemory sharedMem;
	QLocalServer *localServer;
	static const int timeout = 1000;
};

#endif // SINGLEAPPLICATION_H
