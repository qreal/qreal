#include "patternparser.h"
#include "../../qrkernel/ids.h"
#include "pattern.h"
#include <QtXml>
#include <QtCore/QPointF>

using namespace qReal;

PatternParser::PatternParser()
{
}

void PatternParser::loadXml(QString xml){
        mXml = xml;
        mXml.replace("\\\"", "\"");
        mXml.replace("\\n", "\n");
        }

void PatternParser::parseGroups(QString ed, QString diag){
     //       mPatterns.removeAll();
            QDomDocument doc;
            if (mXml.isNull()){
                qDebug() << "ERROR: no xml-file to parse";
                return;
            }
            mEditor = ed;
            mDiagram = diag;
            doc.setContent(mXml);
            QDomElement groups = doc.firstChildElement("groups");

            if (groups.isNull())
            {
                qDebug() << "ERROR: groups tag not found";
                return;
            }
            for (QDomElement group = groups.firstChildElement("group"); !group.isNull();
                group = group.nextSiblingElement("group"))
                parseGroup(group);
        }

QList<Pattern> PatternParser::getPatterns(){
            return mPatterns;
        }

void PatternParser::parseGroup(QDomElement group){
            Pattern pattern;
            pattern.setName(group.attribute("name"));
            pattern.setInNode(group.attribute("inNode"));
            pattern.setOutNode(group.attribute("outNode"));

            for (QDomElement node = group.firstChildElement("node"); !node.isNull();
                node = node.nextSiblingElement("node"))
                parseNode(node, pattern);

            for (QDomElement edge = group.firstChildElement("edge"); !edge.isNull();
                edge = edge.nextSiblingElement("edge"))
                parseEdge(edge, pattern);

           pattern.setEditor(mEditor);
           pattern.setDiagram(mDiagram);
            mPatterns.operator +=(pattern);

}

void PatternParser::parseNode(QDomElement node, Pattern &pattern){
            QString type = node.attribute("type");
            QString name = node.attribute("name");
            float x = node.attribute("xPosition").toFloat();
            float y = node.attribute("yPosition").toFloat();
            QPointF pos = QPointF(x,y);
            pattern.addNode(type, name, pos);
        }

void PatternParser::parseEdge(QDomElement edge, Pattern &pattern){
                QString type;
                QString from, to;
                type = edge.attribute("type");
                from = edge.attribute("from");
                to = edge.attribute("to");
                pattern.addEdge(type, from, to);
        }


