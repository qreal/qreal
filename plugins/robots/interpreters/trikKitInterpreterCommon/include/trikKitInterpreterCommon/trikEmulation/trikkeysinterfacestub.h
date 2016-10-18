#pragma once

#include <trikControl/keysInterface.h>

class TrikKeysInterfaceStub : public trikControl::KeysInterface {


	// DeviceInterface interface
public:
	virtual Status status() const override { return Status::off; }

	// KeysInterface interface
public slots:
	virtual void reset() override {}
	virtual bool wasPressed(int code) override {}
	virtual bool isPressed(int code) override {}
	virtual int buttonCode(bool wait) override {}
};
