#include "trikV62PascalABCGeneratorPlugin.h"

#include <trikKit/blocks/trikV62BlocksFactory.h>
#include <trikGeneratorBase/robotModel/trikV62GeneratorRobotModel.h>

using namespace trik;
using namespace trik::pascalABC;
using namespace kitBase::robotModel;

TrikV62PascalABCGeneratorPlugin::TrikV62PascalABCGeneratorPlugin()
	: TrikPascalABCGeneratorPluginBase(initModel(
				new robotModel::TrikV62GeneratorRobotModel(
						"trikV62Kit"
						, "trikKitRobot"
						, "TrikV62PascalABCGeneratorRobotModel"
						, tr("Generation (PascalABC)")
						, 8 /* After Javascript generator */))
				, new blocks::TrikV62BlocksFactory()
				, {":/trikPascalABC/templates"})
{
}

QString TrikV62PascalABCGeneratorPlugin::kitId() const
{
	return "trikV62Kit";
}

void TrikV62PascalABCGeneratorPlugin::init(const kitBase::KitPluginConfigurator &configurator)
{
	TrikPascalABCGeneratorPluginBase::init(configurator);
}

robotModel::TrikV62GeneratorRobotModel *TrikV62PascalABCGeneratorPlugin::initModel(
		robotModel::TrikV62GeneratorRobotModel * const model)
{
	mModel = model;
	return model;
}

