#include "numericWidget.h"

NumericWidget::NumericWidget(Element *element, const QString &propertyName, QWidget *parent) :
	QSpinBox(parent),
	Control(propertyName),
	element(element)
{
	connect(
		this, SIGNAL(valueChanged(int)),
		this, SLOT(setNewValue(int))
	);
}

NumericWidget* NumericWidget::castToWidget() const {
	return const_cast<NumericWidget*>(this);
}

void NumericWidget::setNewValue(const int value) const {
	element->setLogicalProperty(propertyName, QString::number(value));
}
