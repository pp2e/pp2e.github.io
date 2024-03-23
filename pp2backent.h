#ifndef PP2BACKENT_H
#define PP2BACKENT_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

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
    QML_NAMED_ELEMENT(Backend)

public:
    explicit PP2Backent(QObject *parent = nullptr);
    ~PP2Backent();

    // Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen NOTIFY fullscreenChanged FINAL)

    Q_INVOKABLE bool loadPresentationFromFile();
    Q_INVOKABLE bool unloadPresentation();
    Q_INVOKABLE void printType(QObject *object);

signals:
    void allDone();
};

#endif // PP2BACKENT_H
