#pragma once

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QListIterator>
#include <QtGui/QPixmap>
#include <QtWidgets/QListWidgetItem>

#include "../tools/tool.h"
#include "../tools/root.h"
#include "toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class ToolFactory
{

public:
	static ToolFactory *instance();

	QListIterator<Tool *> itemsIterator();
	Tool *makeItem(QString const &tag, ToolController *controller);
	Root *makeRoot(ToolController *controller) const;
	QPixmap widgetPixmap(QString const &tag);
	QWidget *deserializeWidget(QDomElement const &element
			, QList<PropertyEditorInterface *> &editors);
	Root *loadDocument(ToolController *controller, QDomDocument const &document);
	Tool *loadElement(LayoutTool *parent, QDomElement const &element
		, ToolController *controller);

private:
	ToolFactory();
	ToolFactory(ToolFactory const&) {}

	// Hard-coded tool enumeration; TODO: "pluginize" it
	void initTags();
	void initItems();
	QWidget *deserializeWidget(QWidget *parent, QDomElement const &element
			, QList<PropertyEditorInterface *> &editors);

	QList<QString> mTags;
	QList<Tool *> mItems;
};

}
}
