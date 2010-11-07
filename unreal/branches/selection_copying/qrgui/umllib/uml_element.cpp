#include "uml_element.h"

#include <QtGui>

#include "../model/model.h"

using namespace qReal;
using namespace UML;

Element::Element()
	: mMoving(false)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
		ItemClipsToShape | ItemSendsGeometryChanges);
	setAcceptDrops(true);
	setCursor(Qt::PointingHandCursor);
}

void Element::setIndex(QPersistentModelIndex &index)
{
	mDataIndex = index;
	mUuid = mDataIndex.data(roles::idRole).value<Id>();
	update();
}

Id Element::uuid() const
{
	return mUuid;
}

void Element::updateData()
{
	setToolTip(mDataIndex.data(Qt::ToolTipRole).toString());
}

QPersistentModelIndex Element::index() const
{
	return mDataIndex;
}

QList<ContextMenuAction*> Element::contextMenuActions()
{
	return QList<ContextMenuAction*>();
}

int Element::roleIndexByName(QString const &roleName) const
{
	model::Model const *itemModel = static_cast<model::Model const *>(mDataIndex.model());
	QStringList properties = itemModel->assistApi().editorManager().getPropertyNames(uuid().type());
	return properties.indexOf(roleName) + roles::customPropertiesBeginRole;
}

QString Element::roleValueByName(QString const &roleName) const
{
	int roleIndex = roleIndexByName(roleName);
	if (roleIndex < roles::customPropertiesBeginRole)
	// we'd better check (in generators) that we're binding on an existing field here
		return ""; 
		// Надо бы проверять в генераторе, что мы биндимся на существующее поле, а то будет как в сильверлайте.
	return mDataIndex.model()->data(mDataIndex, roleIndex).toString();
}

void Element::setRoleValueByName(QString const &roleName, QString const &value)
{
	int roleIndex = roleIndexByName(roleName);
	if (roleIndex < roles::customPropertiesBeginRole)
		return;
	QAbstractItemModel *itemModel = const_cast<QAbstractItemModel*>(mDataIndex.model());
	itemModel->setData(mDataIndex, value, roleIndex);
}

