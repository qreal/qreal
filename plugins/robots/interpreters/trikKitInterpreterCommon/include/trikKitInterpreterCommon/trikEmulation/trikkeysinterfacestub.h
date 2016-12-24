#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QScopedPointer>
#include <QtCore/QHash>

#include <utils/abstractTimer.h>
#include <trikControl/keysInterface.h>

namespace kitBase {
namespace robotModel {
namespace robotParts {
class Button;
}
}
}

namespace trik {

namespace robotModel {
namespace twoD {
class TrikTwoDRobotModel;
}
}

class TrikKeysInterfaceStub : public trikControl::KeysInterface {

public:
	TrikKeysInterfaceStub(const QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> &model);

	void init();
	void stop();
	virtual Status status() const override { return Status::ready; }

public slots:
	virtual void reset() override;
	virtual bool wasPressed(int code) override;
	virtual bool isPressed(int code) override;
	virtual int buttonCode(bool wait) override;

//private slots:
//	void buttonChanged(int code, int value);

private:
	bool registerButton(int code);

	QHash<int, QString> mKeycodeMap;
	QHash<int, bool> mWasPressed;
	QHash<int, kitBase::robotModel::robotParts::Button *> mButtons;
	QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> mTwoDRobotModel;
	QScopedPointer<utils::AbstractTimer> mButtonWatchingTimer;
};

}
