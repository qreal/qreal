#pragma once

#include "control.h"
#include "../element.h"

#include <QCheckBox>

class BooleanWidget : public QCheckBox, public Control
{
	Q_OBJECT
	public:
		BooleanWidget(Element *element, const QString &propertyName, QWidget *parent = 0);
	private:
		Element *element;
	private slots:
		void setNewValue(const bool value) const;
};
