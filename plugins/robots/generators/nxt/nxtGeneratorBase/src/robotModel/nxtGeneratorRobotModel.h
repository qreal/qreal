#pragma once

#include <nxtKit/robotModel/nxtRobotModelBase.h>

namespace nxt {
namespace robotModel {

class NxtGeneratorRobotModel : public NxtRobotModelBase
{
	Q_OBJECT

public:
	NxtGeneratorRobotModel(const QString &kitId, const QString &robotId
			, const QString &name, const QString &friendlyName, int priority);

	QString name() const override;
	QString friendlyName() const override;

	bool needsConnection() const override;
	bool interpretedModel() const override;

	int priority() const override;

private:
	const QString mName;
	const QString mFriendlyName;
	const int mPriority;
};

}
}
