#pragma once

#include <QtCore/QObject>

#include <interpreterBase/kitPluginInterface.h>
#include "nullRobotModel.h"

namespace nullKitInterpreter {

class NullKitInterpreterPlugin : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nullKitInterpreter.NullKitInterpreterPlugin")

public:
	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	// Transfers ownership.
	interpreterBase::AdditionalPreferences *settingsWidget() override;

	// Transfers ownership.
//	virtual QList<QWidget *> toolWidgets() const;

//	qReal::IdList specificBlocks() const override;

	qReal::IdList unsupportedBlocks() const override;

	QList<qReal::ActionInfo> customActions() const override;

private:
	NullRobotModel mRobotModel;
};

}
