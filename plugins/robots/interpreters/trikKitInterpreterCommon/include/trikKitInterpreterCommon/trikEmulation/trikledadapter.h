/* Copyright 2016-2017 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

