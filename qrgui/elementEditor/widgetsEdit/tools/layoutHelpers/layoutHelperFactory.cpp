#include "layoutHelperFactory.h"

using namespace Ui::WidgetsEdit;

LayoutHelperFactory::LayoutHelperFactory(LayoutTool *tool)
	: mTool(tool), mType(NoLayout)
{
	mGridHelper = new GridLayoutHelper(tool);
	mHorizontalHelper = new LinearLayoutHelper(tool, Qt::Horizontal);
	mVerticalHelper = new LinearLayoutHelper(tool, Qt::Vertical);
}

LayoutType LayoutHelperFactory::currentType() const
{
	return mType;
}

QString LayoutHelperFactory::layoutTypeToString(LayoutType const type)
{
	switch(type) {
	case Grid:
		return "Grid";
	case Horizontal:
		return "Horizontal";
	case Vertical:
		return "Vertical";
	case NoLayout:
		return "NoLayout";
	}
	return "Unknown";
}

void LayoutHelperFactory::setLayout(LayoutType const type)
{
	mType = type;

	LayoutHelperBase *helper;
	QGraphicsLayout *layout;
	switch(type) {
	case Grid:
		helper = mGridHelper;
		layout = new QGraphicsGridLayout;
		break;
	case Horizontal:
		helper = mHorizontalHelper;
		layout = new QGraphicsLinearLayout(Qt::Horizontal);
		break;
	case Vertical:
		helper = mVerticalHelper;
		layout = new QGraphicsLinearLayout(Qt::Vertical);
		break;
	default:
		helper = NULL;
		layout = NULL;
		break;
	}

	// TODO: call only needed disconnection
	mGridHelper->disconnectHelper();
	mVerticalHelper->disconnectHelper();
	mHorizontalHelper->disconnectHelper();
	if (helper) {
		helper->resetLayout(layout);
	} else {
		mTool->setLayout(NULL);
		mTool->setMinimumSize(0, 0);
	}
	mTool->setLayoutHelper(helper);
}
