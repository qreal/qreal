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

#include <QtXml/QDomElement>
#include <QtCore/QStringList>

#include <qrutils/scalableItem.h>

namespace utils {
	class OutFile;
}

class Port : public utils::ScalableItem
{
public:
	virtual bool init(const QDomElement &element, int width, int height) = 0;
	virtual void generateCode(utils::OutFile &out, const QStringList &portTypes) = 0;
	virtual Port *clone() const = 0;

	QString type() const;

protected:
	QString mType;
};
