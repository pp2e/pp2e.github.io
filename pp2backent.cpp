#include "pp2backent.h"

#include <QFile>
#include <QDebug>
#include <QResource>

#ifdef Q_OS_WASM
#include <QtGui/private/qwasmlocalfileaccess_p.h>
#include <private/qstdweb_p.h>

#include <emscripten/val.h>
#include <emscripten/bind.h>
#endif

PP2Backent::PP2Backent(QObject *parent)
: QObject(parent) {
    m_rccData = new QByteArray();

#ifdef Q_OS_WASM
    auto fullscreenCallback = [=] (emscripten::val event) {
        emit fullscreenChanged();
    };
    m_fullscreenConnection = new qstdweb::EventCallback(emscripten::val::global("document"), "fullscreenchange", fullscreenCallback);
#endif
}

PP2Backent::~PP2Backent()
{
    qDebug() << "deletete";
    delete m_rccData;
#ifdef Q_OS_WASM
    delete m_fullscreenConnection;
#endif
}

bool PP2Backent::loadPresentationFromFile()
{
#ifdef Q_OS_WASM
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
#endif
    return true;
}

void PP2Backent::setFullscreen(bool newFullscreen)
{
    // if (m_fullscreen != newFullscreen) {
    //     m_fullscreen = newFullscreen;

#ifdef Q_OS_WASM
        using emscripten::val;
        const val document = val::global("document");
        const val fullscreenElement = document["fullscreenElement"];
        if (fullscreenElement.isUndefined() || fullscreenElement.isNull())
            document["documentElement"].call<void>("requestFullscreen");
        else
            document.call<void>("exitFullscreen");
#endif

        // emit fullscreenChanged();
    // }
}

bool PP2Backent::fullscreen() const
{
    // return m_fullscreen;
#ifdef Q_OS_WASM
        using emscripten::val;
        const val document = val::global("document");
        const val fullscreenElement = document["fullscreenElement"];
        if (fullscreenElement.isUndefined() || fullscreenElement.isNull())
            return false;
        return true;
#endif
    return false;
}
