/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "patternParser.h"

#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QtCore/QPointF>

#include <qrkernel/ids.h>

#include "plugins/pluginManager/editorManager.h"
#include "plugins/pluginManager/pattern.h"

using namespace qReal;

PatternParser::PatternParser()
{
}

void PatternParser::loadXml(const QString &xml)
{
	mXml = xml;
	mXml.replace("\\\"", "\"");
	mXml.replace("\\n", "\n");
}

void PatternParser::parseGroups(EditorManager *editorManager, const QString &editor, const QString &diagram)
{
	QDomDocument doc;
	if (mXml.isNull()){
		qDebug() << "ERROR: pattern parser: no xml-file to parse";
		return;
	}

	mEditorManager = editorManager;
	mEditor = editor;
	mDiagram = diagram;
	doc.setContent(mXml);
	QDomElement groups = doc.firstChildElement("groups");
	if (groups.isNull()) {
		return;
	}

	for (QDomElement group = groups.firstChildElement("group"); !group.isNull()
			; group = group.nextSiblingElement("group"))
	{
		parseGroup(group);
	}
}

QList<Pattern> PatternParser::patterns() const
{
	return mPatterns;
}

void PatternParser::parseGroup(const QDomElement &group)
{
	Pattern pattern(mEditor, mDiagram, group.attribute("name")
			, group.attribute("inNode"), group.attribute("outNode")
			, group.attribute("rootNode"));

	for (QDomElement node = group.firstChildElement("groupNode"); !node.isNull()
			; node = node.nextSiblingElement("groupNode"))
	{
		parseNode(node, pattern);
	}

	for (QDomElement edge = group.firstChildElement("groupEdge"); !edge.isNull()
			; edge = edge.nextSiblingElement("groupEdge"))
	{
		parseEdge(edge, pattern);
	}

	pattern.countSize(mEditorManager);
	mPatterns += pattern;
}

void PatternParser::parseNode(const QDomElement &node, Pattern &pattern)
{
	const qreal x = node.attribute("xPosition").toFloat();
	const qreal y = node.attribute("yPosition").toFloat();
	const QString parent = node.attribute("parent");
	const QPointF pos = QPointF(x, y);
	pattern.addNode(node.attribute("type"), node.attribute("name"), pos, parent);
}

void PatternParser::parseEdge(const QDomElement &edge, Pattern &pattern)
{
	pattern.addEdge(edge.attribute("type"), edge.attribute("from"), edge.attribute("to"));
}
