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

class Property
{
public:
	bool init(const QDomElement &element);
	QString name() const;
	QString displayedName() const;
	QString type() const;
	QString defaultValue() const;
	QString description() const;
	bool isReferenceProperty() const;
	Property *clone();
	bool operator == (const Property &other) const;
	bool operator != (const Property &other) const;
	void dump() const;

private:
	QString mName;
	QString mDisplayedName;
	QString mType;
	bool mIsEnum;
	bool mIsReference;
	QString mDescription;
	QString mDefaultValue;

	bool initReferenceType(QString typeName, const QDomElement &element);
};
