#include "layoutedControl.h"

#include <QLabel>
#include <QDebug>
#include <QBoxLayout>

LayoutedControl::LayoutedControl(EmbeddedControl *control) :
	QWidget(),
	control(control),
	layout(new QBoxLayout(QBoxLayout::TopToBottom))
{
	QWidget* widget = control->castToWidget();
	layout->addWidget(new QLabel(control->getPropertyName()));
	layout->addWidget(widget);
	layout->setAlignment(widget, Qt::AlignCenter);
	layout->setSpacing(0);
	setLayout(layout);
}

EmbeddedControl* LayoutedControl::getControl() const {
	return control;
}
