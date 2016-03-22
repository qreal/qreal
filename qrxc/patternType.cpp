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

#include "patternType.h"

#include <QtCore/QDebug>

#include <qrutils/outFile.h>

#include "diagram.h"
#include "nameNormalizer.h"

using namespace utils;

PatternType::PatternType(Diagram *diagram, const QString &xml)
	: GraphicType(diagram)
	, mXml(xml)
{
}

PatternType::~PatternType()
{
}

Type *PatternType::clone() const
{
	PatternType *result = new PatternType(mDiagram, mXml);
	GraphicType::copyFields(result);
	return result;
}

void PatternType::generateCode(OutFile &out)
{
	const QString className = NameNormalizer::normalize(qualifiedName());

	out() << "\tclass " << className << " : public qReal::PatternType\n\t{\n"
			<< "\tpublic:\n";

	out() << "\t\texplicit " << className << "(qReal::Metamodel &metamodel)\n"
			<< "\t\t\t: PatternType(metamodel)\n"
			<< "\t\t{\n";
	generateCommonData(out);
	out() << "\t\t\tsetXml(QString::fromUtf8(\"" << mXml << "\"));\n"
			<< "\t\t}\n\n";

	out() << "\t};";
	out() << "\n\n";
}

bool PatternType::init(const QDomElement &element, const QString &context)
{
	mElement = element;
	return Type::init(element, context);
}

bool PatternType::initGraphics()
{
	return true;
}

bool PatternType::initAssociations()
{
	return true;
}

bool PatternType::initDividability()
{
	return true;
}

bool PatternType::initPortTypes()
{
	return true;
}

bool PatternType::initLabel(Label *label, const QDomElement &element, const int &count)
{
	Q_UNUSED(label)
	Q_UNUSED(element)
	Q_UNUSED(count)
	return true;
}

bool PatternType::copyPorts(NodeType *parent)
{
	Q_UNUSED(parent)
	return true;
}

bool PatternType::copyPictures(GraphicType *parent)
{
	Q_UNUSED(parent)
	return true;
}
