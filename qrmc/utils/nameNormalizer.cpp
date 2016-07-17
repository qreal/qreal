/* Copyright 2007-2016 QReal Research Group, Dmitry Mordvinov, Yurii Litvinov
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

#include "qrutils/stringUtils.h"

QString NameNormalizer::normalize(const QString &name)
{
	QString result = name;
	if (name.contains("::")) {
		result = result.right(result.length() - result.lastIndexOf("::") - 2);
	}

	result = result.simplified().replace(" ", "_");
	result = upperFirst(result);
	while (result.endsWith("_")) {
		result.chop(1);
	}

	return result;
}

QString NameNormalizer::upperFirst(const QString &string)
{
	const QStringList tokens = string.split("_", QString::SkipEmptyParts);
	QStringList upperedTokens;
	for (const QString &token : tokens) {
		upperedTokens.append(utils::StringUtils::capitalizeFirstLetter(token));
	}

	return upperedTokens.join("");
}
