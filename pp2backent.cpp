#include "pp2backent.h"

#include <QFile>
#include <QDebug>
#include <QResource>

#ifdef Q_OS_WASM
#include <QtGui/private/qwasmlocalfileaccess_p.h>

#include <emscripten/val.h>
#include <emscripten/bind.h>
#endif

PP2Backent::PP2Backent(QObject *parent)
: QObject(parent) {
    m_rccData = new QByteArray();
}

PP2Backent::~PP2Backent()
{
    qDebug() << "deletete";
    delete m_rccData;
}

bool PP2Backent::loadPresentationFromFile()
{
    QWasmLocalFileAccess::openFile("*.rcc",
        [](int fileCount) { Q_ASSERT(fileCount == 1); },
        [this](uint64_t size, const std::string name) -> char * {
            qDebug() << name;
            m_rccData->resize(size);
            return m_rccData->data();
        },
        [this](){
            qDebug() << "File imported" << m_rccData->size();
            if (QResource::registerResource(reinterpret_cast<const uchar*>(m_rccData->data()), "/presentation/")) {
                qDebug() << "AllRight";
                emit allDone();
            }

    });
    return true;
}

void PP2Backent::setFullscreen(bool newFullscreen)
{
    if (m_fullscreen != newFullscreen) {
        m_fullscreen = newFullscreen;

        using emscripten::val;
        const val document = val::global("document");
        const val fullscreenElement = document["fullscreenElement"];
        if ((fullscreenElement.isUndefined() || fullscreenElement.isNull()) && m_fullscreen)
            document["documentElement"].call<void>("requestFullscreen");
        else
            document.call<void>("exitFullscreen");

        emit fullscreenChanged();
    }
}

bool PP2Backent::fullscreen() const
{
    return m_fullscreen;
}
