/* Copyright 2007-2016 QReal Research Group
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

#include "portFactory.h"

#include <QtCore/QList>

using namespace qReal::gui::editor;

// Using two list.map can replace this whole class :(

QList<StatPoint *> PortFactory::createPorts(const QList<PointPortInfo> &infos) const
{
	QList<StatPoint *> result;
	for (const PointPortInfo &info : infos) {
		result << new StatPoint(info);
	}

	return result;
}

QList<StatLine *> PortFactory::createPorts(const QList<LinePortInfo> &infos) const
{
	QList<StatLine *> result;
	for (const LinePortInfo &info : infos) {
		result << new StatLine(info);
	}

	return result;
}
