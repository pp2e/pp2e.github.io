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

    ResourceLibrary *m_reslib = nullptr;
};

class LilrccNetworkReply : public QNetworkReply {
public:
    explicit LilrccNetworkReply(QObject *parent = nullptr);
    ~LilrccNetworkReply();

    qint64 readData(char *data, qint64 maxlen);
    void abort();
};

#endif // LILRCCNETWORKACCESSMANAGER_H
