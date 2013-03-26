#include <QDomElement>
#include <QDomDocument>
#include <QtCore/QPointF>

#include "patternParser.h"
#include "../../qrkernel/ids.h"
#include "pattern.h"

using namespace qReal;

PatternParser::PatternParser()
{
}

void PatternParser::loadXml(QString const &xml)
{
	mXml = xml;
	mXml.replace("\\\"", "\"");
	mXml.replace("\\n", "\n");
}

void PatternParser::parseGroups(QString const &editor, QString const &diagram)
{
	QDomDocument doc;
	if (mXml.isNull()){
		qDebug() << "ERROR: no xml-file to parse";
		return;
	}
	mEditor = editor;
	mDiagram = diagram;
	doc.setContent(mXml);
	QDomElement groups = doc.firstChildElement("groups");
	if (groups.isNull()){
		qDebug() << "ERROR: groups tag not found";
		return;
	}
	for (QDomElement group = groups.firstChildElement("group"); !group.isNull();
		 group = group.nextSiblingElement("group"))
	{
		parseGroup(group);
	}
}

QList<Pattern> PatternParser::getPatterns()
{
	return mPatterns;
}

void PatternParser::parseGroup(QDomElement const &group)
{
	Pattern pattern = Pattern(mEditor, mDiagram,
							  group.attribute("name"), group.attribute("inNode"), group.attribute("outNode"));
	for (QDomElement node = group.firstChildElement("groupNode"); !node.isNull();
		 node = node.nextSiblingElement("groupNode"))
	{
		parseNode(node, pattern);
	}
	for (QDomElement edge = group.firstChildElement("groupEdge"); !edge.isNull();
		 edge = edge.nextSiblingElement("groupEdge"))
	{
		parseEdge(edge, pattern);
	}
	pattern.countSize();
	mPatterns.operator +=(pattern);

}

void PatternParser::parseNode(QDomElement const &node, Pattern &pattern)
{
	float x = node.attribute("xPosition").toFloat();
	float y = node.attribute("yPosition").toFloat();
	QPointF const pos = QPointF(x,y);
	pattern.addNode(node.attribute("type"), node.attribute("name"), pos);
}

void PatternParser::parseEdge(QDomElement const &edge, Pattern &pattern)
{
	pattern.addEdge(edge.attribute("type"), edge.attribute("from"), edge.attribute("to"));
}
