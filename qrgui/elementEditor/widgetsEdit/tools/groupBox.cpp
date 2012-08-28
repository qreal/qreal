#include "groupBox.h"

#include <QGroupBox>

using namespace Ui::WidgetsEdit;

GroupBox::GroupBox(ToolController *controller)
	: LayoutTool(new QGroupBox("groupBox"), controller)
{
	mGroupBox = dynamic_cast<QGroupBox *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/groupBox.png");
	mTitle = "Group Box";
	mGroupBox->setGeometry(0, 0
		, GROUP_BOX_DEFAULT_WIDTH
		, GROUP_BOX_DEFAULT_HEIGHT);
}
