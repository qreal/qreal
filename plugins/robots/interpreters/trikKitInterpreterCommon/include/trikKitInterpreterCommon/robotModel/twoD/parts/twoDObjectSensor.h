/* Copyright 2007-2015 QReal Research Group
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

#include <QtCore/qmath.h>
#include <trikKit/robotModel/parts/trikObjectSensor.h>
#include <twoDModel/engine/twoDModelEngineInterface.h>

#include "trikKitInterpreterCommon/declSpec.h"
#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h"

namespace trik {
namespace robotModel {
namespace twoD {
namespace parts {

/// Stub for TRIK color sensor for interpreter.
/// Does nothing until object sensor emulation will be requested for 2D model.
class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT ObjectSensor : public robotModel::parts::TrikObjectSensor
{
	Q_OBJECT

public:
	ObjectSensor(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port, twoDModel::engine::TwoDModelEngineInterface &engine);

	void init() override;
	void detect() override;
	void read() override;

	void setDisplay(trik::robotModel::twoD::parts::Display * display);

protected:
	bool wall2Trap(const QPointF &pos, const QPointF &direct,
				   const QPointF &wall_start, const QPointF &wall_end, Trap &result);

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
	trik::robotModel::twoD::parts::Display *mDis;

	/// Angle of view
	const qreal view_angle = M_PI/3;
	const qreal sin_view = qSin(view_angle / 2);
	const qreal cos_view = qCos(view_angle / 2);

	QPair<qreal, qreal> get_intersect(const QPointF &direct, const QPointF &pt1, const QPointF &pt2);
	QPointF convert_coord(const QPointF &pos, const QPointF &direct, const QPointF &pt);
	QPair<qreal, qreal> get_distance(const QPointF &pt);
	Trap Trap2display(qreal direct1, qreal dist1, qreal direct2, qreal dist2);

};

}
}
}
}
