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

signals:
    void allDone();

private:
    QByteArray m_rccData2;
    bool m_registered;
};

#endif // PP2BACKENT_H
