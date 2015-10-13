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

#include <QtCore/QVariant>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT Number
{
public:
	enum Type {
		doubleType = 0
		, intType
	};

public:
	Number(const QVariant &value, Type type);
	Number();
	virtual ~Number();

	Type type() const;
	void setType(Type type);

	virtual QVariant value() const;
	void setValue(const QVariant &value);

	QStringList toStringList() const;

	void operator+=(const Number &add);
	void operator-=(const Number &sub);
	void operator*=(const Number &mult);
	void operator/=(const Number &div);
	Number operator-();
	bool operator<(const Number &arg);
	bool operator>(const Number &arg);
	bool operator==(const Number &arg);
	bool operator<=(const Number &arg);
	bool operator>=(const Number &arg);
	bool operator!=(const Number &arg);

protected:
	QVariant mValue;
	Type mType;
};

}
