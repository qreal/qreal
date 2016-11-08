#pragma once

#include <trikControl/ledInterface.h>

#include <trikKit/robotModel/parts/trikLed.h>

namespace trik {

class TrikLedAdapter : public trikControl::LedInterface
{
	Q_OBJECT

public:
	TrikLedAdapter(trik::robotModel::parts::TrikLed *led);

	virtual Status status() const override { return Status::ready; }

public slots:
	virtual void red() override;
	virtual void green() override;
	virtual void orange() override;
	virtual void off() override;

private:
	trik::robotModel::parts::TrikLed *mLed;

};

}

