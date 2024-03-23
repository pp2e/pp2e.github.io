#ifndef PP2INSPECTOR_H
#define PP2INSPECTOR_H

#include <QObject>

class QQuickItem;
class PP2Inspector : public QObject
{
    Q_OBJECT
public:
    explicit PP2Inspector(QObject *parent = nullptr);

    Q_INVOKABLE void printAllTexts(QObject *target, qreal opacity = 1);
    Q_INVOKABLE void printBindings(QObject *target);

signals:
};

#endif // PP2INSPECTOR_H
