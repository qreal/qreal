#pragma once

#include "control.h"
#include "../element.h"

#include <QSpinBox>

class NumericWidget : public QSpinBox, public Control
{
	Q_OBJECT
	public:
		NumericWidget(Element *element, const QString &propertyName, QWidget *parent = 0);
	private:
		Element *element;
	private slots:
		void setNewValue(const int value) const;
};
