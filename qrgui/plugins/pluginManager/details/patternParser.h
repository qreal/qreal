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
	void parseGroup(QDomElement const &group);
	void parseNode(QDomElement const &node, Pattern &pattern);
	void parseEdge(QDomElement const &edge, Pattern &pattern);

	QString mXml;
	QString mEditor;
	QString mDiagram;
	EditorManager *mEditorManager;
	QList<Pattern> mPatterns;

};
}
