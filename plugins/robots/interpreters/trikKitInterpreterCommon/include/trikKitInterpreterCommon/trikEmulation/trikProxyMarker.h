/* Copyright 2017 CyberTech Labs Ltd.
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

#include <trikControl/markerInterface.h>

#include <twoDModel/robotModel/parts/marker.h>

namespace trik {

class TrikProxyMarker : public trikControl::MarkerInterface
{
	Q_OBJECT

public:
	TrikProxyMarker(twoDModel::robotModel::parts::Marker *marker);

	Status status() const override {return Status::ready;}

	Q_INVOKABLE virtual void down(const QString &color) override;
	Q_INVOKABLE virtual void up() override;
	Q_INVOKABLE virtual bool isDown() const override;
	Q_INVOKABLE virtual void setDown(bool isDown) override;

private:
	twoDModel::robotModel::parts::Marker *mMarker; // Doesn't have ownership.
};

}
