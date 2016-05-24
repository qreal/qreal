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

#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDObjectSensor.h"

#include "src/engine/items/wallItem.h"
#include "twoDModel/engine/model/worldModel.h"
#include "src/engine/twoDModelEngineApi.h"

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;

ObjectSensor::ObjectSensor(const DeviceInfo &info, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikObjectSensor(info, port)
	, mEngine(engine)
	, mDis(nullptr)
{
}

void ObjectSensor::setDisplay(trik::robotModel::twoD::parts::Display * display)
{
	mDis = display;
}


void ObjectSensor::init()
{
}

void ObjectSensor::detect()
{
	Trap cur_Trap;
	QList<Trap> TrapList;
	const QList<QPair<QPointF, QPointF>> walls = mEngine.walls();
	const QPair<QPointF, qreal> neededPosDir = mEngine.countPositionAndDirection(port());
	const QPointF position = neededPosDir.first;
	const qreal direction = neededPosDir.second;
	const QPointF dir(qCos(direction/ 180.0 * M_PI), qSin(direction / 180.0 * M_PI));
	QPair<QPointF, QPointF> wall;
	foreach(wall, walls) {
		if (wall2Trap(position, dir, wall.first, wall.second, cur_Trap))
			TrapList.push_back(cur_Trap);
	}

	if (mDis)
	mDis->drawObjectView(TrapList);
}

void ObjectSensor::read()
{
	emit newData({});
}

bool ObjectSensor::wall2Trap(const QPointF &pos, const QPointF &direct
		, const QPointF &wall_start, const QPointF &wall_end, Trap &result)
{
	const QPointF p1 = convert_coord(pos, direct, wall_start);
	const QPointF p2 = convert_coord(pos, direct, wall_end);

	const QPair<qreal, qreal> dist1 = get_distance(p1);
	const QPair<qreal, qreal> dist2 = get_distance(p2);

	const QPair<qreal, qreal> intersect1 = get_intersect(QPointF(cos_view, sin_view), p1, p2);
	const QPair<qreal, qreal> intersect2 = get_intersect(QPointF(cos_view, -sin_view), p1, p2);

	if(dist1.second >= 0 && dist2.second >= 0) {
		result = Trap2display(dist1.first, dist1.second, dist2.first, dist2.second);
	} else if (dist1.second < 0 && dist2.second >= 0) {
		if(intersect1.second > 0){
			result = Trap2display(intersect1.first, intersect1.second, dist2.first, dist2.second);
		} else {
			result = Trap2display(dist2.first, dist2.second, intersect2.first, intersect2.second);
		}
		} else if(dist1.second >= 0 && dist2.second < 0) {
			if(intersect1.second > 0) {
				result = Trap2display(intersect1.first, intersect1.second, dist1.first, dist1.second);
			} else {
				result = Trap2display(dist1.first, dist1.second, intersect2.first, intersect2.second);
			}
			} else {    /// if(dist1.second < 0 && dist2.second < 0)
				if(intersect1.second > 0 && intersect2.second > 0){
					result = Trap2display(intersect1.first, intersect1.second, intersect2.first, intersect2.second);
				} else {
					return false;
				}
				}

	return true;
}

/// return <sin(angle), dist>
QPair<qreal, qreal> ObjectSensor::get_intersect(const QPointF &direct, const QPointF &pt1, const QPointF &pt2)
{
	qreal dist_max = qMax(sqrt(pt1.x() * pt1.x() + pt1.y() * pt1.y())
						 , sqrt(pt2.x() * pt2.x() + pt2.y() * pt2.y()));

	// start1 = (0,0), end1 = dir1
	// start2 = pt1, end2 = pt2

	const QPointF dir1 = dist_max * direct;
	const QPointF dir2 = pt2 - pt1;

	const qreal a1 = - dir1.y();
	const qreal b1 = + dir1.x();
	const qreal d1 = 0;    //-(a1*start1.x + b1*start1.y);

	const qreal a2 = - dir2.y();
	const qreal b2 = + dir2.x();
	const qreal d2 = - (a2 * pt1.x() + b2 * pt1.y());

	//
	const qreal seg1_line2_start = a2*0/*start1.x*/ + b2*0/*start1.y*/ + d2;
	const qreal seg1_line2_end = a2*dir1.x()/*end1.x*/ + b2*dir1.y()/*end1.y*/ + d2;

	const qreal seg2_line1_start = a1*pt1.x()/*start2.x*/ + b1*pt1.y()/*start2.y*/ + d1;
	const qreal seg2_line1_end = a1*pt2.x()/**end2.x*/ + b1*pt2.y()/*end2.y*/ + d1;

	if (seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0){
		return QPair<qreal, qreal>(0, -1);
	}

	qreal u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
	//*out_intersection =  start1 + u*dir1;
	return QPair<qreal, qreal>(direct.y(), u * dist_max);
}

/// Move and rotatre  "-direct" angle

QPointF ObjectSensor::convert_coord(const QPointF &pos, const QPointF &direct, const QPointF &pt)
{
	const QPointF p = pt - pos;
	const qreal s = -direct.y();
	const qreal c = direct.x();
	return QPointF(p.x() * c - p.y() * s, p.x() * s + p.y() * c);
}

/// return <sin(angle), dist>
QPair<qreal, qreal> ObjectSensor::get_distance(const QPointF &pt)
{
	if (pt.x() <= 0){
		return QPair<qreal, qreal>(0, -1);
	}

	const qreal dist = sqrt(pt.x() * pt.x() + pt.y() * pt.y());
	if(dist == 0){
		return QPair<qreal, qreal>(0, 0);
	}
	if (qAbs(pt.y() / dist) > sin_view){
		return QPair<qreal, qreal>(0, -1);
	}

	return QPair<qreal, qreal>(pt.y() / dist, dist);
}

/// Bijection [0..1] -> [0..100] on X
///	[200..0] -> [0..100] on y
/// if distance is more than 200 on y then wall is too far away
Trap ObjectSensor::Trap2display(qreal direct1, qreal dist1, qreal direct2, qreal dist2)
{
	Trap currentTrap;

	currentTrap.x1 = 50 - direct1 / sin_view * 50.0;
	currentTrap.y1 = qMax(0.0, qMin (100.0, 100 - dist1 / 10.0));
	currentTrap.x2 = 50 - direct2 / sin_view * 50.0;
	currentTrap.y2 = qMax(0.0, qMin (100.0, 100 - dist2 / 10.0));

	return currentTrap;
}
