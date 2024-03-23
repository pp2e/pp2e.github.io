#include <QQmlApplicationEngine>
#ifdef Q_OS_WASM
#include <QGuiApplication>
#else
#include <QApplication>
#endif
#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>

class MyNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    QNetworkAccessManager *create(QObject *parent) override;
};

QNetworkAccessManager *MyNetworkAccessManagerFactory::create(QObject *parent)
{
    QNetworkAccessManager *nam = new QNetworkAccessManager(parent);

    qDebug() << "created noom";

    return nam;
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_WASM
    QGuiApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif

    app.setApplicationName("PowerPoint2");
    app.setApplicationDisplayName("PowerPoint2");

    QQmlApplicationEngine engine;
    MyNetworkAccessManagerFactory namFactory;
    engine.setNetworkAccessManagerFactory(&namFactory);
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
