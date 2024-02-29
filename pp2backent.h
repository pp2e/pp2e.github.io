#ifndef PP2BACKENT_H
#define PP2BACKENT_H

#include <QObject>

namespace qstdweb {
    class EventCallback;
}
class PP2Backent : public QObject
{
    Q_OBJECT

public:
    explicit PP2Backent(QObject *parent = nullptr);
    ~PP2Backent();

    Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen NOTIFY fullscreenChanged FINAL)

    Q_INVOKABLE bool loadPresentationFromFile();


    void setFullscreen(bool newFullscreen);
    bool fullscreen() const;

signals:
    void allDone();
    void fullscreenChanged();

private:
    QByteArray *m_rccData;
    // bool m_fullscreen = false;
    qstdweb::EventCallback *m_fullscreenConnection;
};

#endif // PP2BACKENT_H
