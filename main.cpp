#include <QGuiApplication>
#include <QCursor>
#include <QQuickItem>
#include <QQmlContext>
#include <QDebug>

#include "singleapplication.h"
#include "mainview.h"
#include "windowfunc.h"
#include "guihandler.h"

int main(int argc, char *argv[])
{
	SingleApplication app(argc, argv, "Equalizer APO Manager");
	if (app.isRunning()){
		app.sendMsg("FOCUS");
		exit(0);
	}

	QCursor cursor;
	MainView view;
	view.setFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	view.setTitle("Equalizer APO Manager");
	view.setIcon(QIcon(":/Resources/Icon.png"));

	WindowFunc windowFunc(&cursor, &view);
	GuiHandler guiHandler(&view);
	QQmlContext *context = view.rootContext();
	context->setContextProperty("windowFunc", &windowFunc);
	context->setContextProperty("guiHandler", &guiHandler);
	context->setContextProperty("comboBoxModel", QStringList());
	view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));

	guiHandler.comboBox = view.rootObject()->findChild<QObject*>("presetComboBox");
	guiHandler.bandsGroup = view.rootObject()->findChild<QObject*>("bandsGroup");

	guiHandler.load();

	QObject::connect(&app, SIGNAL(msgAvailable(QString)),
					 &view, SLOT(on_receiveMsg(QString)));

	view.show();

	return app.exec();
}
