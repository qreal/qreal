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

#include "nameNormalizer.h"

#include <QtCore/QStringList>
#include <QtCore/QDebug>

QString NameNormalizer::normalize(const QString &name)
{
	QString result = name;
	if (name.contains("::"))
		result = result.right(result.length() - result.lastIndexOf("::") - 2);
	result = result.simplified().replace(" ", "_");
	result = result.replace("::", "_");
	result = upperFirst(result);
	while (result.endsWith("_"))
	{
		result.chop(1);
	}
	return result;
}

QString NameNormalizer::upperFirst(const QString &string)
{
	if (string.size() < 1)
	{
		return "";
	}
	QStringList tokens = string.split(" ");
	QStringList upperedTokens;
	foreach (QString token, tokens)
	{
		upperedTokens.append(token.at(0).toUpper() + token.mid(1));
	}
	return upperedTokens.join("_");
}
