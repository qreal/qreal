/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

namespace qrmc {

/// Represents enumeration --- non-graphical type that denotes a set of predefined values. And can be a type of
/// a property.
class EnumType : public NonGraphicType
{
public:
	/// Constructor.
	/// @param diagram - diagram this enum belongs to.
	/// @param api - repository with metamodel.
	/// @param id - id of this element in metamodel.
	EnumType(const Diagram &diagram, const qrRepo::LogicalRepoApi &api, const qReal::Id &id);

	bool init(const QString &context) override;
	Type* clone() const override;

	void print() override;

	QString generateEnums(const QString &lineTemplate) const override;

private:
	QMap<QString, QString> mValues;
};

}
