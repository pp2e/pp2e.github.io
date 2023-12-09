#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "pp2backent.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName("PowerPoint2");
    app.setApplicationDisplayName("PowerPoint2");

    qmlRegisterType<PP2Backent>("PowerPoint2", 1, 0, "Backend");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/PowerPoint2/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
