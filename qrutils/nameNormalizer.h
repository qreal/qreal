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

#include <QtCore/QString>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// Helper class that is meant to transform arbitrary string to a correct C++/C# identifier.
class QRUTILS_EXPORT NameNormalizer
{
public:
	/// Makes from given name correct C++/C# identifier.
	/// @param name String to transform to an identifier.
	/// @param upperFirst Makes first letter capital when true or small when false.
	/// @return Transformed string.
	static QString normalize(const QString &name, const bool upperFirst = true);

	/// Makes from given name correct C++/C# identifier. Illegal characters would
	/// be thrown away
	/// @param name String to transform to an identifier.
	/// @param upperFirst Makes first letter capital when true or small when false.
	/// @return Transformed string.
	static QString normalizeStrongly(const QString &name, const bool upperFirst = true);

private:
	/// Makes all letters that start words capital and replaces spaces with underscore,
	/// for example tranforms "some text" to "Some_Text".
	static QString upperFirst(const QString &string);

	/// Makes all letters that start words lower and replaces spaces with underscore,
	/// for example tranforms "some text" to "some_text".
	static QString lowerFirst(const QString &string);

	/// Spells given id in translit-style (e.g. 'идентификатор' -> 'identificator')
	static QString russianTranslit(const QString &russianString);
};

}
