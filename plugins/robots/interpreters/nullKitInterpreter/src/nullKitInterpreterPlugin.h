#pragma once

#include <QtCore/QTranslator>

#include <interpreterBase/kitPluginInterface.h>
#include "nullRobotModel.h"

namespace nullKitInterpreter {

class NullKitInterpreterPlugin : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nullKitInterpreter.NullKitInterpreterPlugin")

public:
	NullKitInterpreterPlugin();

	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	interpreterBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			interpreterBase::robotModel::RobotModelInterface const *model) override;

	// Transfers ownership.
	interpreterBase::AdditionalPreferences *settingsWidget() override;

	// Transfers ownership.
//	virtual QList<QWidget *> toolWidgets() const;


	QList<qReal::ActionInfo> customActions() override;

	QIcon iconForFastSelector(interpreterBase::robotModel::RobotModelInterface const &robotModel) const override;

private:
	NullRobotModel mRobotModel;
	QTranslator mAppTranslator;
};

}
