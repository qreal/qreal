#include "ev3KitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

using namespace ev3KitInterpreter;
using namespace qReal;

Id const robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
Id const subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

Ev3KitInterpreterPlugin::Ev3KitInterpreterPlugin()
		: mRealRobotModel(kitId())
		, mBlocksFactory(new blocks::Ev3BlocksFactory)
{
	mAdditionalPreferences = new Ev3AdditionalPreferences(mRealRobotModel.name());

	connect(mAdditionalPreferences, &Ev3AdditionalPreferences::settingsChanged
		, &mRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);

}

QString Ev3KitInterpreterPlugin::kitId() const
{
	return "ev3Kit";
}

QString Ev3KitInterpreterPlugin::friendlyKitName() const
{
	return tr("EV3");
}

QList<interpreterBase::robotModel::RobotModelInterface *> Ev3KitInterpreterPlugin::robotModels()
{
	return  {&mRealRobotModel};
}

interpreterBase::blocksBase::BlocksFactoryInterface *Ev3KitInterpreterPlugin::blocksFactoryFor(
		interpreterBase::robotModel::RobotModelInterface const *model)
{
	Q_UNUSED(model)
	return mBlocksFactory;
}

QList<interpreterBase::AdditionalPreferences *> Ev3KitInterpreterPlugin::settingsWidgets()
{
	return { mAdditionalPreferences };
}

QList<qReal::ActionInfo> Ev3KitInterpreterPlugin::customActions()
{
	return {};
}

QList<qReal::HotKeyActionInfo> Ev3KitInterpreterPlugin::hotKeyActions()
{
	return {};
}

QIcon Ev3KitInterpreterPlugin::iconForFastSelector(
		interpreterBase::robotModel::RobotModelInterface const &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon();
}
