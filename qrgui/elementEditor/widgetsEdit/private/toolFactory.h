#pragma once

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QListIterator>
#include <QtGui/QPixmap>
#include <QtGui/QListWidgetItem>

#include "../tools/tool.h"
#include "../tools/root.h"
#include "toolController.h"

namespace qReal
{
namespace widgetsEdit
{

struct TitleTagPair
{
public:
	TitleTagPair(QString const &title, QString const &tagName)
	{
		this->title = title;
		this->tagName = tagName;
	}

	QString title;
	QString tagName;
};

class ToolFactory
{

public:
	static ToolFactory *instance();

	QListIterator<Tool *> itemsIterator();
	Tool *makeItem(QString const &title, ToolController *controller);
	Root *makeRoot(ToolController *controller) const;
	QPixmap widgetPixmap(QString const &title);
	QWidget *deserializeWidget(QDomElement const &element);
//	QWidget *deserializeWidget(const QDomElement &element
//		, const QDomElement &shape);

	QString toolTitleToTagName(QString const &title) const;
	QString tagNameToToolTitle(QString const &tagName) const;

private:
	ToolFactory();
	ToolFactory(ToolFactory const&) {}

	//hard-coded tool enumeration
	void initTitles();
	void initItems();
	QWidget *deserializeWidget(QWidget *parent, QDomElement const &element);

	QList<TitleTagPair > mTitles;
	QList<Tool *> mItems;
};

}
}
