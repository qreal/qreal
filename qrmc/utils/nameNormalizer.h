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

#include <QtCore/QString>

/// Helper class that allows to transform a string to valid C++ identifier.
class NameNormalizer
{
public:
	NameNormalizer() = delete;

	/// Returns valid C++ identifier by given name. It replaces spaces with "_", completely removes left parts of "::"
	/// and transforms string to CamelCase style.
	static QString normalize(const QString &name);

private:
	/// Transforms string into CamelCase using "_" as a separator, for example "camel_case" will be transformed to
	/// "CamelCase".
	static QString upperFirst(const QString &string);
};
