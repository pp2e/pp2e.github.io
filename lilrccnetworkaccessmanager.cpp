#include "lilrccnetworkaccessmanager.h"

#include <QDebug>
#include <QFile>
#include <QNetworkReply>
#include <QVariant>
#include <QTimer>

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
    // QString error;
    // m_reslib->ls("/Krim/", error);
    // QByteArray data = m_reslib->getFile()
}

LilrccNetworkAccessManager::~LilrccNetworkAccessManager() {
    if (m_reslib) delete m_reslib;
}

QNetworkReply *LilrccNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) {
    qDebug() << op;
    QUrl url = request.url();
    if (url.scheme() == "lilrcc") {
        qDebug() << "return lilrcc reply";
        QNetworkReply *reply = new LilrccNetworkReply(request, this);
        return reply;
    }
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}

LilrccNetworkReply::LilrccNetworkReply(QByteArray data, QObject *parent)
    : QNetworkReply(parent)
    , m_data(data)
{
    setOpenMode(ReadOnly);
    QTimer::singleShot(0, this, SIGNAL(finished()));
}

LilrccNetworkReply::~LilrccNetworkReply() {
    qDebug() << "destr";
}

void LilrccNetworkReply::abort() {}

qint64 LilrccNetworkReply::bytesAvailable() const {
    return m_data.size()-m_offset;
}

qint64 LilrccNetworkReply::readData(char *data, qint64 maxlen) {
    if (m_offset >= m_data.size())
        return -1;

    int len = m_data.size()-m_offset;
    if (maxlen < len) len = maxlen;
    memcpy(data, m_data.constData() + m_offset, len);
    m_offset += len;
    return len;
}
