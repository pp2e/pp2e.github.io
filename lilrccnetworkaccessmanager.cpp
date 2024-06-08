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
    m_device = new QFile("/home/eugen/Projects/dddd.rcc");
    m_device->open(QIODeviceBase::ReadOnly);
    m_reader = new ResourceReader(m_device);
    m_reslib = new ResourceLibrary(m_reader);
}

LilrccNetworkAccessManager::~LilrccNetworkAccessManager() {
    if (m_reslib) delete m_reslib;
    if (m_reader) delete m_reader;
    if (m_device) delete m_device;
}

QNetworkReply *LilrccNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) {
    QUrl url = request.url();
    qDebug() << url;
    if (url.scheme() == "lilrcc") {
        QString error;
        QByteArray data = m_reslib->getFile(url.path(), error);
        if (!error.isEmpty()) qDebug() << "cannot read:" << error;
        QNetworkReply *reply = new LilrccNetworkReply(data, this);
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
