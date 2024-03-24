#include "pp2rccmodel.h"

#include <QDir>

Resource::Resource(const QString &file, const QLocale &locale)
    : QResource(file, locale) {}

int Resource::childrenCount()
{
    return children().size();
}

QString Resource::childAt(int index)
{
    return children().at(index);
}

bool Resource::hasChild(QString name)
{
    return children().contains(name);
}

ResourceNode::ResourceNode(const QString &path,
                           ResourceNode *parentNode)
    : m_path(path)
    , m_parentNode(parentNode)
{
}

ResourceNode::~ResourceNode()
{
    if (m_children.size())
        qDeleteAll(m_children.values());
}

QString ResourceNode::fileName()
{
    return m_path.mid(m_path.lastIndexOf("/")+1);
}

QString ResourceNode::filePath()
{
    return m_path;
}

// ResourceNode *ResourceNode::childAt(int index)
// {
//     return m_children.values().at(index);
// }

// ResourceNode *ResourceNode::child(QString name)
// {
//     return m_children.value(name);
// }

// void ResourceNode::refreshChild(QString name)
// {
//     Resource res(filePath());
//     m_children.remove(name);
//     int index = res.childIndex(name);
//     if (index == -1) return;
//     m_children.insert(name, new ResourceNode(m_path + "/" + name,
//                                             this));
// }

// void ResourceNode::refreshAll()
// {
//     m_children.clear();
//     // call childCount to load childrens
//     if (!m_children.size()) {
//         Resource res(filePath());
//         for (int i = 0; i < res.childrenCount(); i++) {
//             m_children.insert(res.childAt(i),
//                                     new ResourceNode(filePath() + "/" + res.childAt(i),
//                                                      this));
//         }
//     }
// }

int ResourceNode::placeFor(QString name)
{
    int i;
    for (i = 0; i < m_children.size(); i++) {
        if (m_children.keys().at(i) >= name)
            return i;
    }
    return i;
}

ResourceNode *ResourceNode::parentNode()
{
    return m_parentNode;
}

int ResourceNode::index()
{
    if (!m_parentNode) return 0;
    return m_parentNode->indexOf(this);
}

int ResourceNode::indexOf(ResourceNode *child)
{
    return m_children.values().indexOf(child);
}

static PP2RccModel *s_rccModel;

PP2RccModel::PP2RccModel() {
    s_rccModel = this;
    m_rootResourceNode = new ResourceNode();
}

PP2RccModel::~PP2RccModel()
{
    delete m_rootResourceNode;
}

PP2RccModel *PP2RccModel::getInstance()
{
    return s_rccModel;
}

QModelIndex PP2RccModel::index(int row, int column, const QModelIndex &parent) const
{
    ResourceNode *node = getNode(parent);
    // qDebug() << "index" << row << node->filePath() << node->m_children.values().at(row)->filePath();
    // qDebug() << "index" << node->childAt(row) << row << node->childCount() << node->fileName();
    if (row >= node->m_children.size()) {
        qCritical() << "Trying to get" << row << "of" << node->m_children.size() << "childs of" << node->filePath();
        return QModelIndex();
    }
    return createIndex(row, column, node->m_children.values().at(row));
}

// Note: if this returns something other then invalid when parentNode == m_rootResourceNode app will crash
QModelIndex PP2RccModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) return QModelIndex();

    ResourceNode *node = getNode(index);
    ResourceNode *parentNode = node->parentNode();
    return getIndex(parentNode);
}

int PP2RccModel::rowCount(const QModelIndex &index) const
{
    ResourceNode *node = getNode(index);
    if (!node->m_children.size()) {
        Resource res(node->filePath());
        for (int i = 0; i < res.childrenCount(); i++) {
            node->m_children.insert(res.childAt(i),
                              new ResourceNode(node->filePath() + "/" + res.childAt(i),
                                               node));
        }
    }
    // qDebug() << "rowCount" << node->filePath() << node->m_children.size();
    return node->m_children.size();
}

int PP2RccModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant PP2RccModel::data(const QModelIndex &index, int role) const
{
    ResourceNode *node = getNode(index);
    return node->fileName();
}


bool PP2RccModel::addResource(const QByteArray *byteArray, const QString &name)
{
    if (name.isEmpty()) {
        qWarning() << "Resource name should not be empty!!";
        return false;
    }
    if (name.contains("/")) {
        qWarning() << "Resource name should not contain path symbol('/')!!";
        return false;
    }
    PP2RccModel *model = getInstance();
    if (model->m_resources.contains(name)) {
        qWarning("Resource with such name already exists(%s)!!", qUtf8Printable(name));
        return false;
    }

    uchar *data = new uchar[byteArray->size()];
    std::memcpy(data, byteArray->data(), byteArray->size());
    bool result = QResource::registerResource(data, "/mount/"+name);
    if (!result) {
        delete[] data;
        return false;
    }
    model->m_resources.insert(name, data);

    ResourceNode *rootNode = model->m_rootResourceNode;
    if (!rootNode->m_children.contains("mount")) {
        int index = rootNode->placeFor("mount");
        model->beginInsertRows(model->getIndex(rootNode), index, index);
        rootNode->m_children.insert("mount", new ResourceNode("/mount",
                                                              rootNode));
        model->endInsertRows();
    }
    ResourceNode *node = model->m_rootResourceNode->m_children.value("mount");
    Resource res(node->filePath());
    if (res.hasChild(name)) {
        // find the right place for item
        int index = node->placeFor(name);
        model->beginInsertRows(model->getIndex(node), index, index);
        node->m_children.insert(name, new ResourceNode(node->filePath() + "/" + name,
                                                       node));
        model->endInsertRows();
    }
    return true;
}

bool PP2RccModel::removeResource(const QString &name)
{
    if (name.isEmpty()) {
        qWarning() << "Resource name should not be empty!!";
        return false;
    }
    if (name.contains("/")) {
        qWarning() << "Resource name should not contain path symbol('/')!!";
        return false;
    }
    PP2RccModel *model = getInstance();
    if (!model->m_resources.contains(name)) {
        qWarning("Resource with such name is not mounted(%s)!!", qUtf8Printable(name));
        return false;
    }
    bool result = QResource::unregisterResource(model->m_resources.value(name), "/mount/"+name);
    if (!result) return false;
    delete[] model->m_resources.value(name);
    model->m_resources.remove(name);

    ResourceNode *node = model->m_rootResourceNode->m_children.value("mount");
    // find item's place
    int index = node->placeFor(name);
    model->beginRemoveRows(model->getIndex(node), index, index);
    node->m_children.remove(name);
    model->endRemoveRows();

    return true;
}

QModelIndex PP2RccModel::getIndex(ResourceNode *node) const {
    if (!node) return QModelIndex();
    if (node == m_rootResourceNode) return QModelIndex();
    return createIndex(node->index(), 0, node);
}

ResourceNode *PP2RccModel::getNode(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_rootResourceNode;
    return static_cast<ResourceNode*>(index.internalPointer());
}
