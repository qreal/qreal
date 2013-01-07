#include <QtGui/QGroupBox>

#include "groupBox.h"

using namespace qReal::widgetsEdit;

GroupBox::GroupBox(ToolController *controller)
	: LayoutTool(new QGroupBox("groupBox"), controller)
{
	mGroupBox = dynamic_cast<QGroupBox *>(widget());
	mTitle = tr("Group Box");
	mTag = "GroupBox";
	mIcon = QIcon(":/icons/widgetsEditor/groupBox.png");
	mGroupBox->setGeometry(0, 0
		, GROUP_BOX_DEFAULT_WIDTH
		, GROUP_BOX_DEFAULT_HEIGHT);
}

Qt::Alignment GroupBox::titleAlignment() const
{
	return mGroupBox->alignment();
}

bool GroupBox::isFlat() const
{
	return mGroupBox->isFlat();
}

QString GroupBox::title() const
{
	return mGroupBox->title();
}

void GroupBox::setTitleAlignment(Qt::Alignment alignment)
{
	mGroupBox->setAlignment(alignment);
}

void GroupBox::setFlat(bool flat)
{
	mGroupBox->setFlat(flat);
}

void GroupBox::setTitle(const QString &title)
{
	mGroupBox->setTitle(title);
}
