#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QApplication>
#include <QSharedMemory>
#include <QLocalServer>

class SingleApplication :
		public QApplication
{
	Q_OBJECT
public:
	SingleApplication(int &argc, char *argv[], const QString key);
	~SingleApplication();
	bool isRunning();
	bool sendMsg(const QString &message);

public slots:
	void receiveMsg();

signals:
	void msgAvailable(QString msg);

private:
	bool _isRunning;
	QString uniqueKey;
	QSharedMemory sharedMem;
	QLocalServer *localServer;
	static const int timeout = 1000;
};

#endif // SINGLEAPPLICATION_H
