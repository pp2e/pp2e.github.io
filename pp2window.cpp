#include "pp2window.h"

#ifdef Q_OS_WASM
#include <QtGui/private/qwasmlocalfileaccess_p.h>
#include <private/qstdweb_p.h>

#include <emscripten/val.h>
#include <emscripten/bind.h>
#endif

PP2Window::PP2Window(QWindow *parent)
    : QQuickWindow(parent) {
#ifdef Q_OS_WASM
    auto fullscreenCallback = [=] (emscripten::val event) {
        emit fullscreenChanged();
    };
    m_fullscreenConnection = new qstdweb::EventCallback(emscripten::val::global("document"), "fullscreenchange", fullscreenCallback);
    // To not scare user with Qt's internal windows in browser
    showFullScreen();
#else
    connect(this, &QQuickWindow::visibilityChanged,
            this, &PP2Window::fullscreenChanged);
#endif
}

bool PP2Window::fullscreen() const
{
#ifdef Q_OS_WASM
    using emscripten::val;
    const val document = val::global("document");
    const val fullscreenElement = document["fullscreenElement"];
    if (fullscreenElement.isUndefined() || fullscreenElement.isNull())
        return false;
    return true;
#else
    return visibility() & QWindow::FullScreen;
#endif
}

void PP2Window::setFullscreen(bool fullscreen)
{
#ifdef Q_OS_WASM
    using emscripten::val;
    const val document = val::global("document");
    const val fullscreenElement = document["fullscreenElement"];
    if (fullscreenElement.isUndefined() || fullscreenElement.isNull())
        document["documentElement"].call<void>("requestFullscreen");
    else
        document.call<void>("exitFullscreen");
#else
    if (visibility() & QWindow::FullScreen)
        showNormal();
    else
        showFullScreen();
#endif
}
