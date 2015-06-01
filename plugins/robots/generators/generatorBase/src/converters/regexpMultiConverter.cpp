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

#include "generatorBase/converters/regexpMultiConverter.h"

using namespace generatorBase::converters;

RegexpMultiConverter::RegexpMultiConverter(const QString &splitRegexp
		, const simple::Binding::ConverterInterface * const converter)
	: mSplitRegexp(splitRegexp)
	, mSimpleConverter(converter)
{
}

RegexpMultiConverter::~RegexpMultiConverter()
{
	delete mSimpleConverter;
}

QStringList RegexpMultiConverter::convert(const QString &data) const
{
	const QStringList parts = data.toUpper().split(QRegExp(mSplitRegexp), QString::SkipEmptyParts);
	QStringList result;
	for (const QString &part : parts) {
		result << mSimpleConverter->convert(part);
	}

	return result;
}
