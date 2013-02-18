#ifndef PATTERNPARSER_H
#define PATTERNPARSER_H

#include "../../qrkernel/ids.h"
#include "pattern.h"
#include <QtXml/QDomElement>
#include <QtCore/QPointF>


namespace qReal {
class PatternParser
{
public:
    PatternParser();
    void loadXml(QString xml);
    void parseGroups(QString ed, QString diag);
    QList<Pattern> getPatterns();
private:
    QString mXml;
    QString mEditor;
    QString mDiagram;
    QList<Pattern> mPatterns;

    void parseGroup(QDomElement group);
    void parseNode(QDomElement node, Pattern &pattern);
    void parseEdge(QDomElement edge, Pattern &pattern);

};
}
#endif // PATTERNPARSER_H
