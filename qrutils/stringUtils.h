/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// Contains useful methods extending QString class
class QRUTILS_EXPORT StringUtils
{
public:
	/// Splits string into a set of lines and prepends each line with
	/// the given @arg indentString repeated @arg indent times.
	static QString addIndent(const QString &code, int indent, const QString &indentString/* = "\t"*/);

	/// Wraps the given string into double quotes and returns the result.
	static QString wrap(const QString &string);

	/// Returns new instance of string that exactly the same as given but with first letter in upper case.
	static QString capitalizeFirstLetter(const QString &string);

	/// Returns new instance of string that exactly the same as given but with first letter in lower case.
	static QString lowercaseFirstLetter(const QString &string);

	/// Removes a pair of quotes if string starts or end with them.
	static QString dequote(const QString &string);
};

}
