#include <QGuiApplication>
#include <QQuickView>
#include <QCursor>
#include <QQmlContext>

#include "windowfunc.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	QCursor cursor;

	QQuickView view;
	view.setFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	view.setTitle("Equalizer APO Manager");
	view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));

	WindowFunc windowFunc(&cursor, &view);
	QQmlContext *context = view.rootContext();
	context->setContextProperty("windowFunc", &windowFunc);

	view.show();

    return app.exec();
}
