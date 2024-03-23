#include "pp2backent.h"

#include <QFile>
#include <QDebug>
#include <QGuiApplication>

#ifdef Q_OS_WASM
#include <QtGui/private/qwasmlocalfileaccess_p.h>
#include <private/qstdweb_p.h>

#include <emscripten/val.h>
#include <emscripten/bind.h>
#else
#include <QFileDialog>
#include <qquickwindow.h>
#include <qqmlengine.h>
#include "qqmlcontext.h"
#include <qqmlapplicationengine.h>
#endif

#include "pp2rccmodel.h"

PP2Backent::PP2Backent(QObject *parent)
: QObject(parent) {
}

PP2Backent::~PP2Backent()
{
}

bool PP2Backent::loadPresentationFromFile()
{
#ifdef Q_OS_WASM
    QByteArray data;
    QWasmLocalFileAccess::openFile("*.rcc *.pptx2",
        [](int fileCount) { Q_ASSERT(fileCount == 1); },
        [&data](uint64_t size, const std::string name) -> char * {
            qDebug() << name;
            data.resize(size);
            return data.data();
        },
        [data, this](){
            qDebug() << "File imported" << data.size();
            if (PP2RccModel::addResource(&data, "presentation")) {
                emit allDone();
            }
    });
#else
    QFileDialog::getOpenFileContent("*.rcc *.pptx2",
        [this](const QString &fileName, const QByteArray &fileContent) {
            qDebug() << "opened file" << fileName;
            if (PP2RccModel::addResource(&fileContent, "presentation")) {
                emit allDone();
            }
        });
#endif
    return true;
}

bool PP2Backent::unloadPresentation()
{
    return PP2RccModel::removeResource("presentation");
}

void PP2Backent::printType(QObject *object) {
    qDebug() << object->metaObject()->className();
}
