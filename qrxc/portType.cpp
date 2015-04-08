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

#include "portType.h"

#include <qrutils/outFile.h>

bool PortType::init(const QDomElement &element, const QString &context)
{
	return NonGraphicType::init(element, context);
}

Type * PortType::clone() const
{
	PortType *result = new PortType();
	Type::copyFields(result);
	return result;
}

void PortType::generateCode(utils::OutFile &out)
{
	const QString name = Type::name();

	out() << "\tclass " << name << " : public PortImpl\n"
	<< "\t{\n"
	<< "\tpublic:\n"
	<< "\t\t~" << name << "() {}\n"
	<< "\t\tvirtual QString type() const { return \"" << name << "\"; }\n"
	<< "\t};\n\n";
}

bool PortType::generateEnumValues(utils::OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(isNotFirst)
	return true;
}

void PortType::generatePropertyTypes(utils::OutFile &out)
{
	Q_UNUSED(out)
}

void PortType::generatePropertyDefaults(utils::OutFile &out)
{
	Q_UNUSED(out)
}
