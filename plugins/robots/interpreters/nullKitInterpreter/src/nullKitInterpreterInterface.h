#pragma once

#include <QtCore/QObject>

#include <interpreterBase/kitPluginInterface.h>
#include "nullRobotModel.h"

namespace nullKitInterpreter {

class NullKitInterpreterInterface : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nullKitInterpreter.NullKitInterpreterInterface")

public:
	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	// Transfers ownership.
	QWidget *settingsWidget() const override;

	// Transfers ownership.
//	virtual QList<QWidget *> toolWidgets() const;

//	qReal::IdList specificBlocks() const override;

	qReal::IdList unsupportedBlocks() const override;

private:
	NullRobotModel mRobotModel;
};

}
