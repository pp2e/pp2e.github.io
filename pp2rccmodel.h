#ifndef PP2RCCMODEL_H
#define PP2RCCMODEL_H

#include <QResource>
#include <QAbstractItemModel>
#include <QtQml/qqmlregistration.h>

class Resource : public QResource {
public:
    Resource(const QString &file = "", const QLocale &locale = QLocale());

    int childrenCount();
    QString childAt(int index);
    bool hasChild(QString name);
};

class ResourceNode {
public:
    ResourceNode(const QString &file = "",
                 ResourceNode *parentNode = nullptr);
    ~ResourceNode();

    QString fileName();
    QString filePath();

    // ResourceNode *childAt(int index);
    // ResourceNode *child(QString name);
    // void refreshChild(QString name);
    // void refreshAll();
    int placeFor(QString name);

    ResourceNode *parentNode();
    int index();
    int indexOf(ResourceNode *child);



    QMap<QString, ResourceNode*> m_children;

private:
    QString m_path;
    ResourceNode* m_parentNode;
};

class PP2RccModel : public QAbstractItemModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(RccModel)
    QML_SINGLETON

public:
    PP2RccModel();
    ~PP2RccModel();

    static PP2RccModel *getInstance();

public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    // Overload QResource::registerResource with changing this model
    static bool addResource(const QByteArray *byteArray, const QString &name);
    static bool removeResource(const QString &name);

private:
    QModelIndex getIndex(ResourceNode *node) const;
    ResourceNode *getNode(const QModelIndex &index) const;

    QHash<QString, const uchar*> m_resources;
    ResourceNode *m_rootResourceNode;
};

#endif // PP2RCCMODEL_H
