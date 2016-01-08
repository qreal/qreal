/* Copyright 2007-2016 QReal Research Group
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

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QDir>

class Editor;
class Diagram;

namespace utils {
	class OutFile;
}

class XmlCompiler
{
public:
	XmlCompiler();
	~XmlCompiler();
	bool compile(const QString &inputXmlFileName, const QString &sourcesRootFolder);
	Editor *loadXmlFile(const QDir &currentDir, const QString &inputXmlFileName);
	Diagram *getDiagram(const QString &diagramName);
	void addResource(const QString &resourceName);

private:
	void generateCode();
	void generateElementClasses();
	void generatePluginHeader();
	void generatePluginSource();
	void generateIncludes(utils::OutFile &out);
	void generateInitPlugin(utils::OutFile &out);
	void generateNameMappings(utils::OutFile &out);
	void generateNodesAndEdgesSets(utils::OutFile &out);
	void generateMouseGestureMap(utils::OutFile &out);
	void generatePropertyMap(utils::OutFile &out);
	void generatePropertyDefaultsMap(utils::OutFile &out);
	void generateDescriptionMappings(utils::OutFile &out);
	void generatePortTypeMappings(utils::OutFile &out);
	void generateParentsMappings(utils::OutFile &out);
	void generateExplosionsMappings(utils::OutFile &out);
	void generateNameMappingsRequests(utils::OutFile &out);
	void generateGraphicalObjectRequest(utils::OutFile &out);
	void generateIsParentOfRequest(utils::OutFile &out);
	void generateGetParentsOfRequest(utils::OutFile &out);
	void generateProperties(utils::OutFile &out);
	void generatePortTypes(utils::OutFile &out);
	void generateReferenceProperties(utils::OutFile &out);
	void generateContainedTypes(utils::OutFile &out);
	void generatePossibleEdges(utils::OutFile &out);
	void generateNodesAndEdges(utils::OutFile &out);
	void generateGroupsXML(utils::OutFile &out);
	void generateEnumValues(utils::OutFile &out);
	void generateEditableEnums(utils::OutFile &out);
	void generateResourceFile();
	void generatePropertyTypesRequests(utils::OutFile &out);
	void generatePropertyDefaultsRequests(utils::OutFile &out);
	void generatePaletteGroupsLists(utils::OutFile &out);
	void generatePaletteGroupsDescriptions(utils::OutFile &out);
	void generateShallPaletteBeSorted(utils::OutFile &out);

	class ListMethodGenerator;
	class PropertiesGenerator;
	class PortsGenerator;
	class ReferencePropertiesGenerator;
	class ContainedTypesGenerator;
	class PossibleEdgesGenerator;
	class EnumValuesGenerator;

	void generateListMethod(utils::OutFile &out, const QString &signature, const ListMethodGenerator &generator);
	void generateStringSet(utils::OutFile &out, const QSet<QString> &set) const;
	void generateStringList(utils::OutFile &out, const QStringList &list) const;
	void generateStringListMap(utils::OutFile &out, const QMap<QString, QStringList> &map) const;

	QMap<QString, Editor *> mEditors;
	QString mPluginName;
	QString mPluginVersion;
	QString mResources;
	QString mCurrentEditor;
	QString mSourcesRootFolder;
};
