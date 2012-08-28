#include "layoutButton.h"

using namespace Ui::WidgetsEdit;

LayoutButton::LayoutButton(const QString &text, const QIcon &icon
		, const LayoutType type, QWidget *parent)
	: QPushButton(icon, "", parent), mType(type)
{
	setToolTip(text);
	setFlat(true);
	connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

void LayoutButton::onClicked()
{
	emit clicked(mType);
}
