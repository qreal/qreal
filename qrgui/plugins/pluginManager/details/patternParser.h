#pragma once

#include <QtXml/QDomElement>
#include <QtCore/QPointF>

#include <qrkernel/ids.h>

#include "qrgui/plugins/pluginManager/pattern.h"

namespace qReal {
class EditorManager;

class PatternParser
{
public:
	PatternParser();

	void loadXml(const QString &xml);
	void parseGroups(EditorManager *editorManager, const QString &editor, const QString &diagram);
	QList<Pattern> patterns() const;

private:
	void parseGroup(const QDomElement &group);
	void parseNode(const QDomElement &node, Pattern &pattern);
	void parseEdge(const QDomElement &edge, Pattern &pattern);

	QString mXml;
	QString mEditor;
	QString mDiagram;
	EditorManager *mEditorManager;
	QList<Pattern> mPatterns;

};
}
