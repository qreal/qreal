#include "pushButton.h"

using namespace qReal::widgetsEdit;

PushButton::PushButton(ToolController *controller)
	: AbstractButton(new QPushButton("pushButton"), controller)
{
	mPushButton = dynamic_cast<QPushButton *>(widget());
	mTitle = tr("Push Button");
	mTag = "PushButton";
	mIcon = QIcon(":/icons/widgetsEditor/pushButton.png");
}

bool PushButton::isFlat() const
{
	return mPushButton->isFlat();
}

void PushButton::setFlat(bool flat)
{
	mPushButton->setFlat(flat);
}
