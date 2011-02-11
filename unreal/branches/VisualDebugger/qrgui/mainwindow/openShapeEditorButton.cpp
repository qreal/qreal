#include "openShapeEditorButton.h"

#include <QDebug>

OpenShapeEditorButton::OpenShapeEditorButton(QWidget *parent
		, const QPersistentModelIndex &index, const int &role, const QString &propertyValue)
	: QPushButton(parent)
	, mIndex(index)
	, mRole(role)
	, mPropertyValue(propertyValue)
{
}

QPersistentModelIndex OpenShapeEditorButton::index() const
{
	return mIndex;
}

int OpenShapeEditorButton::role() const
{
	return mRole;
}

QString OpenShapeEditorButton::propertyValue() const
{
	return mPropertyValue;
}
