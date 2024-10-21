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
}

LilrccNetworkAccessManager::~LilrccNetworkAccessManager() {
}

QNetworkReply *LilrccNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) {
    QUrl url = request.url();
    // qDebug() << url;

    // auto local = QNetworkAccessManager::createRequest(op, QNetworkRequest({"/gol/phcel"}), outgoingData);

    if (url.scheme() == "lilrcc") {
        // qDebug() << "request";
        // return QNetworkAccessManager::createRequest(op, QNetworkRequest(QUrl("file:/home/eugen/Projects/Krim" + url.path())), outgoingData);
        Lilrcc::Error error = Lilrcc::Error::NoError;
        if (!LilrccLibraryHandler::getInstance()->getLibrary()) {
            qDebug() << "nope";
            return QNetworkAccessManager::createRequest(op, request, outgoingData);
        }

        if (url.fileName() == "qmldir") {
            QString path = url.path().chopped(7);
            qDebug() << "qmldir" << path;
            QList<QString> names = LilrccLibraryHandler::getInstance()->getLibrary()->ls(path, error);
            QString data = "";
            for (QString name : names) {
                if (name.endsWith(".qml"))
                    data += name.chopped(4) + " " + name + "\n";
            }
            QNetworkReply *reply = new LilrccNetworkReply(data.toLocal8Bit(), this);
            return reply;
        }

        QByteArray data = LilrccLibraryHandler::getInstance()->getLibrary()->getFile(url.path(), error);
        // qDebug() << "data" << data;
        if (error != Lilrcc::NoError) {
            Lilrcc::printError(error);
            qDebug() << "error";
            // return QNetworkAccessManager::createRequest(op, QNetworkRequest({"/go/gooo"}), outgoingData);
            return new NotFoundNetworkReply(this);
        }
        // if (!error.isEmpty()) qDebug() << "cannot read:" << error;
        // qDebug() << "lilrcc";
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

NotFoundNetworkReply::NotFoundNetworkReply(QObject *parent)
    : QNetworkReply(parent)
{
    setAttribute(QNetworkRequest::HttpStatusCodeAttribute, 404);
    setAttribute(QNetworkRequest::HttpReasonPhraseAttribute, "Not found");
    setOpenMode(ReadOnly);
    QTimer::singleShot(0, this, SIGNAL(finished()));
}

void NotFoundNetworkReply::abort() {}

qint64 NotFoundNetworkReply::readData(char *data, qint64 maxlen){
    return -1;
}

LilrccLibraryHandler *LilrccLibraryHandler::getInstance() {
    static LilrccLibraryHandler *instance = nullptr;
    if (instance == nullptr)
        instance = new LilrccLibraryHandler();
    return instance;
}

void LilrccLibraryHandler::loadDevice(QIODevice *device){
    m_device = device;
    m_reader = new ResourceReader(m_device);
    m_reslib = new ResourceLibrary(m_reader);
    Lilrcc::Error error;
    m_reslib->ls("", error);
}

ResourceLibrary *LilrccLibraryHandler::getLibrary() {
    return m_reslib;
}

LilrccLibraryHandler::LilrccLibraryHandler() {

}

LilrccLibraryHandler::~LilrccLibraryHandler() {
    if (m_reslib) delete m_reslib;
    if (m_reader) delete m_reader;
    if (m_device) delete m_device;
}
