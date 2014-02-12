#include "pushButton.h"

using namespace qReal::widgetsEdit;

PushButton::PushButton(ToolController *controller)
	: AbstractButton(new QPushButton("pushButton"), controller)
{
	mPushButton = dynamic_cast<QPushButton *>(widget());
	mTitle = tr("Push Button");
	mTag = "PushButton";
	mIcon = QIcon(":/icons/widgetsEditor/pushButton.png");
	mProxy = new PushButtonProxy(mPushButton);
}

PushButtonProxy::PushButtonProxy(QPushButton *pushButton)
	: AbstractButtonProxy(pushButton), mPushButton(pushButton)
{
}

bool PushButtonProxy::isFlat() const
{
	return mPushButton->isFlat();
}

void PushButtonProxy::setFlat(bool flat)
{
	mPushButton->setFlat(flat);
}
