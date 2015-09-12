#include "singleapplication.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	SingleApplication a(argc, argv, "EqualizerAPOManager");
	if (a.isRunning())
	{
		a.sendMsg("FOCUS");
		exit(0);
	}

	MainWindow w;
	QObject::connect(&a, SIGNAL(msgAvailable(QString)), &w, SLOT(on_receiveMsg(QString)));

	QString startArg;
	if (a.arguments().count() != 1) startArg = a.arguments().at(1);

	if (startArg != "-startup") w.show();

	return a.exec();
}
