#ifndef PP2BACKENT_H
#define PP2BACKENT_H

#include <QObject>

#ifdef Q_OS_WASM
namespace qstdweb {
    class EventCallback;
}
#else
class QWindow;
#endif
class PP2Backent : public QObject
{
    Q_OBJECT

public:
    explicit PP2Backent(QObject *parent = nullptr);
    ~PP2Backent();

    // Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen NOTIFY fullscreenChanged FINAL)

    Q_INVOKABLE bool loadPresentationFromFile();


    // void setFullscreen(bool newFullscreen);
    // bool fullscreen() const;

signals:
    void allDone();
    // void fullscreenChanged();

private:
    QByteArray *m_rccData;
// #ifdef Q_OS_WASM
//     qstdweb::EventCallback *m_fullscreenConnection;
// #else
//     QWindow *m_window;
// #endif
};

#endif // PP2BACKENT_H
