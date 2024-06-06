#include "lilrccnetworkaccessmanager.h"

#include <QDebug>
#include <QFile>
#include <QNetworkReply>

QNetworkAccessManager *LilrccNetworkAccessManagerFactory::create(QObject *parent)
{
    QNetworkAccessManager *nam = new LilrccNetworkAccessManager(parent);

    qDebug() << "created little noom";

    return nam;
}

LilrccNetworkAccessManager::LilrccNetworkAccessManager(QObject *parent)
    : QNetworkAccessManager{parent}
{
    QFile file("/home/eugen/Projects/lilrcc/gol.rcc");
    file.open(QIODeviceBase::ReadOnly);
    ResourceReader reader(&file);
    m_reslib = new ResourceLibrary(&reader);
    QString error;
    m_reslib->ls("/Krim/", error);
    // QByteArray data = m_reslib->getFile()
}

LilrccNetworkAccessManager::~LilrccNetworkAccessManager() {
    if (m_reslib) delete m_reslib;
}

QNetworkReply *LilrccNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) {
    qDebug() << request.url();
    QUrl url = request.url();
    if (url.scheme() == "lilrcc") {
        QNetworkReply *reply = new LilrccNetworkReply(0);
    }
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}

LilrccNetworkReply::LilrccNetworkReply(QObject *parent)
    : QNetworkReply(parent)
{}

LilrccNetworkReply::~LilrccNetworkReply() {
    qDebug() << "destr";
}

qint64 LilrccNetworkReply::readData(char *data, qint64 maxlen) {
    return 0;
}

void LilrccNetworkReply::abort() {}
