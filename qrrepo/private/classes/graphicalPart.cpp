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

#include "graphicalPart.h"

#include <qrkernel/exception/exception.h>

#include "qrrepo/private/valuesSerializer.h"

using namespace qrRepo::details;
using namespace qReal;

GraphicalPart::GraphicalPart()
{
}

GraphicalPart::GraphicalPart(const QDomElement &element)
{
	ValuesSerializer::deserializeNamedVariantsMap(mProperties, element);
}

QVariant GraphicalPart::property(const QString &name) const
{
	if (!mProperties.contains(name)) {
		throw Exception("Trying to get non-existing graphical part property " + name);
	}

	return mProperties[name];
}

void GraphicalPart::setProperty(const QString &name, const QVariant &value)
{
	mProperties.insert(name, value);
}

GraphicalPart *GraphicalPart::clone() const
{
	GraphicalPart * const result = new GraphicalPart();
	result->mProperties = this->mProperties;
	return result;
}

QDomElement GraphicalPart::serialize(int index, QDomDocument &document) const
{
	QDomElement result = ValuesSerializer::serializeNamedVariantsMap("graphicalPart", mProperties, document);
	result.setAttribute("index", index);
	return result;
}
