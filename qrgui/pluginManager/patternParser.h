#pragma once

#include <QtXml/QDomElement>
#include <QtCore/QPointF>

#include <qrkernel/ids.h>

#include "pluginManager/pattern.h"

namespace qReal {
class EditorManager;
class PatternParser
{
public:
	PatternParser();
	void loadXml(QString const &xml);
	void parseGroups(EditorManager *editorManager, QString const &editor, const QString &diagram);
	QList<Pattern> patterns() const;

private:
	void parseGroup(QDomElement const &group);
	void parseNode(QDomElement const &node, Pattern &pattern);
	void parseEdge(QDomElement const &edge, Pattern &pattern);

	QString mXml;
	QString mEditor;
	QString mDiagram;
	EditorManager * mEditorManager;
	QList<Pattern> mPatterns;

};
}
