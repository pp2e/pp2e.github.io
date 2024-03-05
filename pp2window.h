#ifndef PP2WINDOW_H
#define PP2WINDOW_H

#include <QQuickWindow>

#ifdef Q_OS_WASM
namespace qstdweb {
class EventCallback;
}
#endif
class PP2Window : public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen NOTIFY fullscreenChanged)
public:
    explicit PP2Window(QWindow *parent = nullptr);

    bool fullscreen() const;
    void setFullscreen(bool fullscreen);

signals:
    void fullscreenChanged();

private:
#ifdef Q_OS_WASM
    qstdweb::EventCallback *m_fullscreenConnection;
#endif
};

#endif // PP2WINDOW_H
