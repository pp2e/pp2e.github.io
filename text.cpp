#include "text.h"

#include <QtGui/qtextlayout.h>
#include <private/qquicktext_p_p.h>

Text::Text() 
 : QQuickText() {

}

Text::~Text() {

}

QList<QList<Symbol>> Text::symbols() {
    QList<QList<Symbol>> symbols;
    for (int i = 0; i < layout()->lineCount(); i++) {
        QTextLine textLine = layout()->lineAt(i);
	QList<QPointF> positions = textLine.glyphRuns()[0].positions();
        QList<Symbol> line;
	//int spacesCount = 0;
        for (int j = 0; j < positions.size(); j++) {
            QString ch4r = layout()->text()[textLine.textStart()+j];
            if (ch4r != " ")
                line.push_back({positions[j].x() + x(), positions[j].y() + y() - textLine.ascent(), 
		    	        //i, layout()->lineCount(),
		    	        //j-spacesCount, positions.size(),
	        	        ch4r});
	    //else spacesCount++;
	}
	// Remove spaces from line size
	//for (int j = 0; j < line.size(); j++)
        //    line[j].m_charsCount -= spacesCount;
	symbols.push_back(line);
    }
    return symbols;
}

QTextLayout *Text::layout() {
    QQuickTextPrivate * priv = reinterpret_cast<QQuickTextPrivate *>(d_ptr.data());
    return &priv->layout;
}
