/* Copyright 2016 Dmitry Mordvinov
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

#include "metaMetaModel/patternType.h"

#include "metaMetaModel/metamodel.h"

using namespace qReal;

PatternType::PatternType(Metamodel &metamodel)
	: ElementType(metamodel)
{
}

ElementType::Type qReal::PatternType::type() const
{
	return Type::pattern;
}

QString PatternType::xml() const
{
	return mXml;
}

void PatternType::setXml(const QString &xml)
{
	mXml = xml;
}
