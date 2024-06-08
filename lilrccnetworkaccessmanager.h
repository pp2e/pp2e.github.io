#ifndef LILRCCNETWORKACCESSMANAGER_H
#define LILRCCNETWORKACCESSMANAGER_H

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "lilrcc/lilrcc.h"
#include "lilrcc/resourcereader.h"
#include "lilrcc/tree.h"

class LilrccNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    QNetworkAccessManager *create(QObject *parent) override;
};

class LilrccNetworkAccessManager : public QNetworkAccessManager
{
public:
    explicit LilrccNetworkAccessManager(QObject *parent = nullptr);
    ~LilrccNetworkAccessManager();

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) override;

    QIODevice *m_device = nullptr;
    ResourceReader *m_reader = nullptr;
    ResourceLibrary *m_reslib = nullptr;
};

class LilrccNetworkReply : public QNetworkReply {
public:
    explicit LilrccNetworkReply(QByteArray data, QObject *parent = nullptr);

    void abort() override;
    qint64 bytesAvailable() const override;

protected:
    qint64 readData(char *data, qint64 maxlen) override;

private:
    int m_offset = 0;
    QByteArray m_data;
};

class NotFoundNetworkReply : public QNetworkReply {
public:
    explicit NotFoundNetworkReply(QObject *parent = nullptr);
    void abort() override;
protected:
    qint64 readData(char *data, qint64 maxlen) override;
};

#endif // LILRCCNETWORKACCESSMANAGER_H
