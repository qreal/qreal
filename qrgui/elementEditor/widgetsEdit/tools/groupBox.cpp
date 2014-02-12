#include "groupBox.h"

using namespace qReal::widgetsEdit;

GroupBox::GroupBox(ToolController *controller)
	: LayoutTool(new QGroupBox("groupBox"), controller)
{
	mGroupBox = dynamic_cast<QGroupBox *>(widget());
	mTitle = tr("Group Box");
	mTag = "GroupBox";
	mIcon = QIcon(":/icons/widgetsEditor/groupBox.png");
	mProxy = new GroupBoxProxy(mGroupBox);
}

GroupBoxProxy::GroupBoxProxy(QGroupBox *groupBox)
	: LayoutToolProxy(groupBox), mGroupBox(groupBox)
{
	mGroupBox->setGeometry(0, 0
		, GROUP_BOX_DEFAULT_WIDTH
		, GROUP_BOX_DEFAULT_HEIGHT);
}

Qt::Alignment GroupBoxProxy::titleAlignment() const
{
	return mGroupBox->alignment();
}

bool GroupBoxProxy::isFlat() const
{
	return mGroupBox->isFlat();
}

QString GroupBoxProxy::title() const
{
	return mGroupBox->title();
}

void GroupBoxProxy::setTitleAlignment(Qt::Alignment alignment)
{
	mGroupBox->setAlignment(alignment);
}

void GroupBoxProxy::setFlat(bool flat)
{
	mGroupBox->setFlat(flat);
}

void GroupBoxProxy::setTitle(const QString &title)
{
	mGroupBox->setTitle(title);
}
