#pragma once

#include <trikKit/robotModel/trikRobotModelBase.h>

namespace trik {
namespace robotModel {

class TrikGeneratorRobotModel : public TrikRobotModelBase
{
	Q_OBJECT

public:
	TrikGeneratorRobotModel(const QString &kitId, const QString &robotId
			, const QString &name, const QString &friendlyName);

	QString name() const override;
	QString friendlyName() const override;

	bool needsConnection() const override;
	bool interpretedModel() const override;

private:
	const QString mName;
	const QString mFriendlyName;
};

}
}
