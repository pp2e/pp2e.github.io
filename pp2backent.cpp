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
    m_rccData = new QByteArray();

// #ifdef Q_OS_WASM
//     auto fullscreenCallback = [=] (emscripten::val event) {
//         emit fullscreenChanged();
//     };
//     m_fullscreenConnection = new qstdweb::EventCallback(emscripten::val::global("document"), "fullscreenchange", fullscreenCallback);
// #else
//     // QQmlApplicationEngine *engine = dynamic_cast<QQmlApplicationEngine*>(qmlEngine(this));
//     // qDebug() << engine;
//     // qDebug() << engine->rootObjects().at(0);
//     // m_window = qobject_cast<QQuickWindow*>(engine->rootObjects().at(0));
//     // connect(m_window, &QWindow::visibilityChanged,
//     //         this, &PP2Backent::fullscreenChanged);
//     // qDebug() << parent;
// #endif
}

PP2Backent::~PP2Backent()
{
    qDebug() << "deletete";
    delete m_rccData;
// #ifdef Q_OS_WASM
//     delete m_fullscreenConnection;
// #else
//     disconnect(m_window, &QWindow::visibilityChanged,
//             this, &PP2Backent::fullscreenChanged);
// #endif
}

bool PP2Backent::loadPresentationFromFile()
{
#ifdef Q_OS_WASM
    QWasmLocalFileAccess::openFile("*.rcc *.pptx2",
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
#else
    QFileDialog::getOpenFileContent("*.rcc *.pptx2",
                                    [this](const QString &fileName, const QByteArray &fileContent) {
                                        qDebug() << "opened file" << fileName;
                                    }
                                    );
#endif
    return true;
}

// void PP2Backent::setFullscreen(bool newFullscreen)
// {
// #ifdef Q_OS_WASM
//         using emscripten::val;
//         const val document = val::global("document");
//         const val fullscreenElement = document["fullscreenElement"];
//         if (fullscreenElement.isUndefined() || fullscreenElement.isNull())
//             document["documentElement"].call<void>("requestFullscreen");
//         else
//             document.call<void>("exitFullscreen");
// #else
//     if (m_window->visibility() & QWindow::FullScreen)
//         m_window->showNormal();
//     else
//         m_window->showFullScreen();
// #endif
// }

// bool PP2Backent::fullscreen() const
// {
// #ifdef Q_OS_WASM
//         using emscripten::val;
//         const val document = val::global("document");
//         const val fullscreenElement = document["fullscreenElement"];
//         if (fullscreenElement.isUndefined() || fullscreenElement.isNull())
//             return false;
//         return true;
// #else
//     return m_window->visibility() & QWindow::FullScreen;
// #endif
//     return false;
// }
