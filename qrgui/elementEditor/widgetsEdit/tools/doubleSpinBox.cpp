#include "doubleSpinBox.h"

using namespace qReal::widgetsEdit;

DoubleSpinBox::DoubleSpinBox(ToolController *controller)
	: AbstractSpinBox(new QDoubleSpinBox, controller)
{
	mDoubleSpinBox = dynamic_cast<QDoubleSpinBox *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/doubleSpinBox.png");
	mTitle = "Double Spin Box";
	connect(mDoubleSpinBox, SIGNAL(valueChanged(double))
		, this, SLOT(valueChanged(double)));
}

void DoubleSpinBox::valueChanged(double d)
{
	emit propertyChanged("value", QVariant(d));
}
