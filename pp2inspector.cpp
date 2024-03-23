#include "pp2inspector.h"

#include <QQuickItem>
#include <private/qquicktext_p.h>
// #include <private/qqmlscriptstring_p.h>
#include <private/qqmldata_p.h>
#include <private/qqmlabstractbinding_p.h>
#include <QQmlProperty>
#include <private/qqmlanybinding_p.h>

PP2Inspector::PP2Inspector(QObject *parent)
    : QObject{parent}
{}

void PP2Inspector::printAllTexts(QObject *target, qreal opacity)
{
    if (!target) return;
    QQmlData *data = QQmlData::get(target);
    if (data)
        qDebug() << target << data->outerContext->baseUrlString() << data->context->baseUrlString() << data->lineNumber << data->columnNumber;
    else {
        QObjectPrivate *priv = QObjectPrivate::get(target);
        qDebug() << target << "Has no data";
    }
    // if (QQuickText *text = qobject_cast<QQuickText*>(target)) {
    //     if (text->isVisible() && opacity*text->opacity() == 1) {
    //         qDebug() << text->parent() << text << text->text();
    //     }
    // }
    for (QObject *object : target->children()) {
        if (QQuickItem *targetItem = qobject_cast<QQuickItem*>(target))
            printAllTexts(object, opacity*targetItem->opacity());
    }
}

void PP2Inspector::printBindings(QObject *target)
{
    QQmlProperty prop(target, "text");
    QQmlAnyBinding anyBinding = QQmlAnyBinding::ofProperty(prop);
    qDebug() << anyBinding.isAbstractPropertyBinding() << anyBinding.isUntypedPropertyBinding();
    QQmlAbstractBinding *absBinding = anyBinding.asAbstractBinding();
    qDebug() << absBinding->expression();
    QQmlBinding *binding = dynamic_cast<QQmlBinding*>(absBinding);
    qDebug() << binding->sourceLocation().sourceFile << binding->sourceLocation().line << binding->expression();
}
