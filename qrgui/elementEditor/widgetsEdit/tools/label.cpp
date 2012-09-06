#include "label.h"

using namespace qReal::widgetsEdit;

Label::Label(ToolController *controller)
	: Frame(new QLabel("label"), controller)
{
	mLabel = dynamic_cast<QLabel *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/label.png");
	mTitle = "Label";
	mLabel->setGeometry(0, 0
		, LABEL_DEFAULT_WIDTH
		, LABEL_DEFAULT_HEIGHT);
}
