#pragma once

#include <private/qquicktext_p.h>

struct Symbol {
    Q_GADGET
    Q_PROPERTY(qreal x READ x)
    Q_PROPERTY(qreal y READ y)
    //Q_PROPERTY(int lineNumber READ lineNumber)
    //Q_PROPERTY(int linesCount READ linesCount)
    //Q_PROPERTY(int charNumber READ charNumber)
    //Q_PROPERTY(int charsCount READ charsCount)
    Q_PROPERTY(QString text READ text)

public:
    qreal x() const { return m_x; }
    qreal y() const { return m_y; }
    //int lineNumber() const { return m_lineNumber; }
    //int linesCount() const { return m_linesCount; }
    //int charNumber() const { return m_charNumber; }
    //int charsCount() const { return m_charsCount; }
    QString text() const { return m_text; }
    // What? Public members? I'm sorry
    qreal m_x, m_y;
    //int m_lineNumber, m_linesCount;
    //int m_charNumber, m_charsCount;
    QString m_text;
};

Q_DECLARE_METATYPE(Symbol)

class QTextLayout;
class Text: public QQuickText {
    Q_OBJECT
    
public:
    Text();
    ~Text();

    Q_INVOKABLE QList<QList<Symbol>> symbols();

    QTextLayout *layout();
};
