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

#include "qrgui/plugins/metaMetaModel/metamodel.h"
#include "qrgui/plugins/pluginManager/interpreterEditorManager.h"
#include "abstractStringGenerator.h"
#include "methodsTester.h"

namespace editorPluginTestingFramework {

class MethodsTesterForQrxcAndQrmc : public MethodsTester
{
public:
	virtual ~MethodsTesterForQrxcAndQrmc() {}

	/// gets editorInterfaces from mainClass and initializes mQrmcGeneratedPlugin and mQrxcGeneratedPlugin with them
	MethodsTesterForQrxcAndQrmc(
			qReal::Metamodel * const qrmcGeneratedPlugin
			, qReal::Metamodel * const qrxcGeneratedPlugin
			);

	/// returns list of generated output to main class
	QList<MethodsTester::ResultOfGenerating> generatedOutput();

	/// clones generator, initializes it with qrxcGeneratedPlugin and returns it to methodsTester
	AbstractStringGenerator * initGeneratorWithFirstInterface(const AbstractStringGenerator &generator) const;

	/// clones generator, initializes it with qrmcGeneratedPlugin and returns it to methodsTester
	AbstractStringGenerator * initGeneratorWithSecondInterface(const AbstractStringGenerator &generator) const;

private:
	class StringGenerator;

	class StringGeneratorForDiagrams;
	class StringGeneratorForElements;
	class StringGeneratorForProperties;
	class StringGeneratorForGroups;

	class PropertiesWithDefaultValuesStringGenerator;
	class TypesContainedByStringGenerator;
	class GetPossibleEdgesStringGenerator;
	class IsNodeOrEdgeStringGenerator;

	class GetPropertyNamesStringGenerator;
	class GetReferencePropertiesStringGenerator;
	class GetParentsOfStringGenerator;

	class GetPropertyTypesStringGenerator;
	class GetPropertyDefaultValueStringGenerator;

	class DiagramNameStringGenerator;
	class DiagramNodeNameStringGenerator;

	class ElementNameStringGenerator;
	class ElementMouseGestureStringGenerator;
	class ElementDescriptionStringGenerator;

	class PropertyDescriptionStringGenerator;
	class PropertyDisplayedNameStringGenerator;

	class IsParentOfStringGenerator;

	class DiagramPaletteGroupListStringGenerator;
	class DiagramPaletteGroupDescriptionStringGenerator;

	class DiagramsStringGenerator;
	class ElementsStringGenerator;
	class ExplosionsStringGenerator;
	class PortTypesStringGenerator;
	class EnumValueStringGenerator;

	/// fills mGeneratedList with results of testing
	void testMethods();

	/// finds out if string contains only of given symbol (for example, "aaa" contains only of symbol 'a')
	static bool containsOnly(const QString &string, QChar const &symbol);

	qReal::Metamodel* mQrmcGeneratedPlugin;
	qReal::Metamodel* mQrxcGeneratedPlugin;

	qReal::InterpreterEditorManager* mInterpreterGeneratedPlugin;

	QList<MethodsTester::ResultOfGenerating> mGeneratedList;
};

}

