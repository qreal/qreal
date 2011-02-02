#include "openShapeEditorButton.h"

#include <QDebug>

OpenShapeEditorButton::OpenShapeEditorButton(QWidget *parent, const QPersistentModelIndex &index, const int &role, const QString &propertyValue)
	: QPushButton(parent), mIndex(index), mRole(role), mPropertyValue(propertyValue)
{
}

QPersistentModelIndex OpenShapeEditorButton::getIndex() const
{
	return mIndex;
}

int OpenShapeEditorButton::getRole() const
{
	return mRole;
}

QString OpenShapeEditorButton::getPropertyValue() const
{
	return mPropertyValue;
}
