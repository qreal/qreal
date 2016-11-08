#include "trikKitInterpreterCommon/trikEmulation/trikkeysinterfacestub.h"

#include <trikKitInterpreterCommon/robotModel/twoD/trikTwoDRobotModel.h>
#include <kitBase/robotModel/robotModelUtils.h>
#include <kitBase/robotModel/robotParts/button.h>

using namespace trik;

TrikKeysInterfaceStub::TrikKeysInterfaceStub(const QSharedPointer<robotModel::twoD::TrikTwoDRobotModel> &model)
	: mTwoDRobotModel(model)
{
	mButtonWatchingTimer.reset(model->timeline().produceTimer());
	connect(mButtonWatchingTimer.data(), &utils::AbstractTimer::timeout, [this](){
		for (const auto &b : mButtons) {
			b->read();
		}
	});//not best since it doesn't know about not yet registed functions
	mButtonWatchingTimer->setInterval(20);
	mButtonWatchingTimer->setRepeatable(true);
	auto codes = model->buttonCodes();
	for (auto it = codes.cbegin(); it != codes.cend(); ++it) {
		mKeycodeMap[it.value()] = it.key().left(it.key().length() - 6); // remove "Button" from the end
		mWasPressed[it.value()] = false;
	}
}

void TrikKeysInterfaceStub::init()
{
	reset();
	if (mButtonWatchingTimer->isTicking()) {
		mButtonWatchingTimer->stop(); // ?
	}
	mButtonWatchingTimer->start();
}

void TrikKeysInterfaceStub::stop()
{
	reset(); // just to be sure;
	mButtonWatchingTimer->stop();
}

void TrikKeysInterfaceStub::reset() {
	for (const auto &b : mButtons) {
		b->read(); // hack to clear lastReadState
	}
	mButtons.clear();
	mWasPressed.clear();
}

bool TrikKeysInterfaceStub::wasPressed(int code)
{
	if (registerButton(code) && mWasPressed[code]) {
		mWasPressed[code] = false;
		return true;
	}
	return false;
}

bool TrikKeysInterfaceStub::isPressed(int code)
{
	if (registerButton(code)) {
		QMetaObject::invokeMethod(mButtons[code], "read");
		return mButtons[code]->lastData(); // race ?
	}
	return false;
}

int TrikKeysInterfaceStub::buttonCode(bool wait)
{
	Q_UNUSED(wait);
	return -1; /// @todo
}

//void TrikKeysInterfaceStub::buttonChanged(int code)
//{
//	int last = mButtons[code]->lastData();
//	if (last != 0) {
//		mWasPressed[code] = true;
//	}
//}

bool TrikKeysInterfaceStub::registerButton(int code)
{
	using namespace kitBase::robotModel;
	if (!mButtons.contains(code)) {
		robotParts::Button * b =
				RobotModelUtils::findDevice<robotParts::Button>(*mTwoDRobotModel, mKeycodeMap[code]);
		if (b == nullptr) {
			qDebug("error, button not found"); // todo - propogate errors to trikbrick
			//emit error(tr("No configured sensor on port: ") + port);
			return false;
		}
		mButtons[code] = b;
		connect(b, &robotParts::Button::newData, [this, code](int value){
			mWasPressed[code] = value != 0;
		});
		//connect(b, &robotParts::Button::newData, this, &TrikKeysInterfaceStub::buttonChanged);
		/// @todo: disconnect somewhere?
	}
	return true;
}
