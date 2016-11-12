#include <QQmlApplicationEngine>
#include "singleapplication.h"
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    SingleApplication app(argc, argv, "EQAPO_Manager");
    if (app.isRunning())
    {
        qDebug() << "Instance running";
        app.sendMessage(SingleApplication::BRINGTOFRONT);
        exit(0);
    }

    QQuickStyle::setStyle("Material");

    bool showWindow = true;
    if (app.arguments().count() != 1 && app.arguments().at(1) == "--startup")
    {
        showWindow = false;
    }

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("showWindow", showWindow);
    engine.rootContext()->setContextProperty("SingleApplication", &app);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
