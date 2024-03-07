#include "pp2backent.h"

#include <QFile>
#include <QDebug>
#include <QResource>

#ifdef Q_OS_WASM
#include <QtGui/private/qwasmlocalfileaccess_p.h>
#include <private/qstdweb_p.h>

#include <emscripten/val.h>
#include <emscripten/bind.h>
#else
#include <QFileDialog>
#include <QApplication>
#include <qquickwindow.h>
#include <qqmlengine.h>
#include "qqmlcontext.h"
#include <qqmlapplicationengine.h>
#endif

PP2Backent::PP2Backent(QObject *parent)
: QObject(parent) {
}

PP2Backent::~PP2Backent()
{
    qDebug() << "deletete";
}

bool PP2Backent::loadPresentationFromFile()
{
#ifdef Q_OS_WASM
    QWasmLocalFileAccess::openFile("*.rcc *.pptx2",
        [](int fileCount) { Q_ASSERT(fileCount == 1); },
        [this](uint64_t size, const std::string name) -> char * {
            qDebug() << name;
            m_rccData->resize(size);
            return m_rccData2.data();
        },
        [this](){
            qDebug() << "File imported" << m_rccData2.size();
            if (m_registered)
                if (!QResource::unregisterResource(reinterpret_cast<const uchar*>(m_rccData2.data()), "/presentation/")) {
                    qFatal() << "Cannot unload old resource while opening new";
                    QGuiApplication.exit(); // Die if cannot unload loaded resource
                }
            if (QResource::registerResource(reinterpret_cast<const uchar*>(m_rccData2.data()), "/presentation/")) {
                qDebug() << "AllRight";
                emit allDone();
            }
    });
#else
    QFileDialog::getOpenFileContent("*.rcc *.pptx2",
        [this](const QString &fileName, const QByteArray &fileContent) {
            qDebug() << "opened file" << fileName;
            if (m_registered)
                if (!QResource::unregisterResource(reinterpret_cast<const uchar*>(m_rccData2.data()), "/presentation/")) {
                    qFatal() << "Cannot unload old resource while opening new";
                    QApplication::exit(); // Die if cannot unload loaded resource
                }
            m_rccData2 = fileContent;
            if (QResource::registerResource(reinterpret_cast<const uchar*>(m_rccData2.data()), "/presentation/"))
                emit allDone();
        });
#endif
    return true;
}
