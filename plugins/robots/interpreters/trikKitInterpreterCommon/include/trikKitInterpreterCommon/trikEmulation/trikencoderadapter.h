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

#include <trikControl/encoderInterface.h>

#include <kitBase/robotModel/portInfo.h> // forward?

namespace twoDModel {
namespace engine {
class TwoDModelEngineInterface;
}
}

namespace trik {

class TrikEncoderAdapter : public trikControl::EncoderInterface
{
	Q_OBJECT
	// DeviceInterface interface
public:
	TrikEncoderAdapter(const kitBase::robotModel::PortInfo &port, twoDModel::engine::TwoDModelEngineInterface *engine);

	virtual Status status() const override {return Status::ready;}
	// EncoderInterface interface
public slots:
	virtual int read() override;
	virtual int readRawData() override { return read(); }
	virtual void reset() override;

private:
	kitBase::robotModel::PortInfo mPort;
	twoDModel::engine::TwoDModelEngineInterface *mEngine;
};

}
