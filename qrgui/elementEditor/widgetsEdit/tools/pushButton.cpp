#include "pushButton.h"

using namespace qReal::widgetsEdit;

PushButton::PushButton(ToolController *controller)
	: Tool(new QPushButton("pushButton"), controller)
{
	mPushButton = dynamic_cast<QPushButton *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/pushButton.png");
	mTitle = "Push Button";
}
