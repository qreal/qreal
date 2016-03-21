#pragma once

#include <trikPascalABCGeneratorLibrary/trikPascalABCGeneratorPluginBase.h>

namespace trik {

namespace robotModel {
class TrikV62GeneratorRobotModel;
}

namespace pascalABC {

class TrikV62PascalABCGeneratorPlugin : public TrikPascalABCGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "trik.TrikV62PascalABCGeneratorPlugin")

public:
	TrikV62PascalABCGeneratorPlugin();

	QString kitId() const override;

	void init(const kitBase::KitPluginConfigurator &configurator) override;

private:
	robotModel::TrikV62GeneratorRobotModel *initModel(robotModel::TrikV62GeneratorRobotModel * const model);

	/// Temporary storage for robot model to be able to correctly initialize it.
	/// Does not have ownership.
	robotModel::TrikV62GeneratorRobotModel *mModel;
};

}
}

