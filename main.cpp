#include <QGuiApplication>
#include <QCursor>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QDebug>

#include "singleapplication.h"
#include "mainview.h"
#include "windowfuncs.h"
#include "guihandler.h"

int main(int argc, char *argv[])
{
	SingleApplication app(argc, argv, "Equalizer APO Manager");
	if (app.isRunning()){
		app.sendMsg("FOCUS");
		exit(0);
	}

	QQmlEngine engine;
	engine.rootContext()->setContextProperty("comboBoxModel", QStringList());
	QQmlComponent mainComponent(&engine, QUrl("qrc:/main.qml"));
	if (mainComponent.status() != mainComponent.Ready){
		if (mainComponent.status() == mainComponent.Error){
			qDebug() << mainComponent.errorString();
		}
	}
	QQuickWindow *mainWindow = qobject_cast<QQuickWindow*>(mainComponent.create());
	mainWindow->setTitle("Equalizer APO Manager");
	mainWindow->setFlags(Qt::Window | Qt::FramelessWindowHint);
	mainWindow->setIcon(QIcon(":/Resources/Icon.png"));

	QCursor cursor;
	WindowFuncs windowFuncs(&cursor, mainWindow);
	GuiHandler guiHandler(mainWindow);

	guiHandler.comboBox = mainWindow->findChild<QObject*>("presetComboBox");
	guiHandler.bands = mainWindow->findChild<QObject*>("bands");
	guiHandler.load();

	QObject::connect(&app, &SingleApplication::msgAvailable,
					 [mainWindow](){mainWindow->show();});
	QObject::connect(mainWindow, SIGNAL(movePressed(int, int, int, int)),
					 &windowFuncs, SLOT(beginMoveWindow(int,int,int,int)));
	QObject::connect(mainWindow, SIGNAL(moveMoved(int, int)),
					 &windowFuncs, SLOT(moveWindow(int,int)));
	QObject::connect(mainWindow, SIGNAL(minimizeButtonClicked()),
					 &windowFuncs, SLOT(minimizeWindow()));
	QObject::connect(mainWindow, SIGNAL(closeButtonClicked()),
					 &windowFuncs, SLOT(closeWindow()));

	QQuickItem *mainItem = mainWindow->findChild<QQuickItem*>("mainForm");
	QObject::connect(mainItem, SIGNAL(powerButtonClicked(bool)),
					 &guiHandler, SLOT(powerToggled(bool)));
	QObject::connect(mainItem, SIGNAL(preampChanged(double)),
					 &guiHandler, SLOT(preampChanged(double)));
	QObject::connect(mainItem, SIGNAL(comboBoxIndexChanged(QString)),
					 &guiHandler, SLOT(comboBoxChanged(QString)));
	QObject::connect(mainItem, SIGNAL(resetButtonClicked()),
					 &guiHandler, SLOT(resetButtonClicked()));
	QObject::connect(mainItem, SIGNAL(saveButtonClicked()),
					 &guiHandler, SLOT(saveButtonClicked()));
	QObject::connect(mainItem, SIGNAL(deleteButtonClicked()),
					 &guiHandler, SLOT(deleteButtonClicked()));
	QObject::connect(mainItem, SIGNAL(bandChanged()),
					 &guiHandler, SLOT(bandChanged()));

	mainWindow->show();

	return app.exec();
}
