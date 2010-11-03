#pragma once

#include <QtGui/QPushButton>
#include <QPersistentModelIndex>

class OpenShapeEditorButton: public QPushButton {

public:
	OpenShapeEditorButton(QWidget *parent, QPersistentModelIndex const &index, int const &role, QString const &propertyValue);

	QPersistentModelIndex getIndex() const;
	int getRole() const;
	QString getPropertyValue() const;

private:
	QPersistentModelIndex const mIndex;
	int const mRole;
	QString const mPropertyValue;
};
