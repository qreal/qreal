#pragma once

#include <QtXml/QDomElement>
#include <QtCore/QPointF>

#include "../../qrkernel/ids.h"
#include "pattern.h"

namespace qReal {
class PatternParser
{
public:
    PatternParser();
    void loadXml(QString xml);
    void parseGroups(QString ed, QString diag);
    QList<Pattern> getPatterns();

private:
    void parseGroup(QDomElement group);
    void parseNode(QDomElement node, Pattern &pattern);
    void parseEdge(QDomElement edge, Pattern &pattern);

	QString mXml;
	QString mEditor;
	QString mDiagram;
	QList<Pattern> mPatterns;

};
}
