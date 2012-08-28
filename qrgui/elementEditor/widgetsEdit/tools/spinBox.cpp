#include "spinBox.h"

using namespace Ui::WidgetsEdit;

SpinBox::SpinBox(ToolController *controller)
	: AbstractSpinBox(new QSpinBox, controller)
{
	mSpinBox = dynamic_cast<QSpinBox *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/spinBox.png");
	mTitle = "Spin Box";
	connect(mSpinBox, SIGNAL(valueChanged(int))
		, this, SLOT(valueChanged(int)));
}

void SpinBox::valueChanged(int i)
{
	emit propertyChanged("value", QVariant(i));
}
