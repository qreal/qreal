#pragma once

#include <QtGui/QPushButton>
#include <QtCore/QPersistentModelIndex>

class OpenShapeEditorButton: public QPushButton {

public:
	OpenShapeEditorButton(QWidget *parent, QPersistentModelIndex const &index, int const &role, QString const &propertyValue);

	QPersistentModelIndex index() const;
	int role() const;
	QString propertyValue() const;

private:
	QPersistentModelIndex const mIndex;
	int const mRole;
	QString const mPropertyValue;
};
