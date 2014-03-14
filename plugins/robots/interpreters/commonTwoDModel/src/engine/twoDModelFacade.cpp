#include "commonTwoDModel/engine/twoDModelFacade.h"

#include "src/engine/d2RobotModel.h"

using namespace twoDModel;

TwoDModelFacade::TwoDModelFacade()
	: mTwoDModelActionInfo(
			new QAction(QIcon(":/icons/2d-model.svg"), QObject::tr("2d model"), nullptr)
			, "interpreters"
			, "tools")
	, mTwoDModel(new D2RobotModel())
{
	/// @todo: why it can't be done in constructor?
	mTwoDModel->createModelWidget();

	QObject::connect(mTwoDModelActionInfo.action(), &QAction::triggered
			, mTwoDModel.data(), &twoDModel::D2RobotModel::showModelWidget);
}

TwoDModelFacade::~TwoDModelFacade()
{
	delete mTwoDModelActionInfo.action();
}

qReal::ActionInfo &TwoDModelFacade::showTwoDModelWidgetActionInfo()
{
	return mTwoDModelActionInfo;
}
