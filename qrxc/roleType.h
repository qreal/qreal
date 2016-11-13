/* Copyright 2016, Julia Khramyshkina QReal Research Group
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

#include "nonGraphicType.h"
#include "property.h"

namespace utils {
	class OutFile;
}

/// Class represents one of the ends of edge.
class RoleType : public NonGraphicType
{
public:
	bool init(const QDomElement &element, const QString &context) override;
	RoleType* clone() const override;

	/// Returns the type of arrow.
	QString typeOfArrow();

	//// Returns list properties of role.
	QList<Property*> getPropertiesOfRole();

	/// I have no idea what is it. Something based on standard UML 2.5.
	bool isEnding();

	/// Returns navigability of role.
	bool isNavigable();

private:
	QString mArrowType;
	QList<Property*> mProperties;
	QString mEnd;
	QString mNavigable;
};
