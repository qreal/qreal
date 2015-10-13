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

#include "qrutils/utilsDeclSpec.h"

#include <QtXml/QDomDocument>

#include <qrgui/mainWindow/errorReporter.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>

namespace utils {

/// This class helps with generation new metamodels based on existing ones. For
/// example copying entire metamodel, insert some new elements, create container
/// or smth with all types in metamodel. Also it helps with generating .pro file
/// and building and loading new plugin.
class QRUTILS_EXPORT MetamodelGeneratorSupport : public QObject
{
	Q_OBJECT

public:
	MetamodelGeneratorSupport(qReal::ErrorReporterInterface *errorReporter
			, qReal::gui::MainWindowInterpretersInterface *mainWindowInterface);

	~MetamodelGeneratorSupport() override;

	/// Compiles and loads to QReal new semantics editor plugin.
	/// @param directoryName - directory where editor was generated (containing .pro file of that editor).
	/// @param metamodelName - metamodel name as set in metamodel.
	/// @param pathToQmake - qmake command.
	/// @param pathToMake - make command.
	/// @param extension - extension of the resulting binary file with editor plugin (.dll/.so).
	/// @param prefix - Optional OS-dependent prefix for resulting binary file (lib for linux, for example).
	void loadPlugin(
			const QString &directoryName
			, const QString &metamodelName
			, const QString &pathToQmake
			, const QString &pathToMake
			, const QString &extension
			, const QString &prefix
	);

	/// Generate .pro file of new editor plugin
	void generateProFile(
			QDomDocument metamodel ///< Metamodel is needed to fix 'include' .xml paths
			, const QString &baseMetamodelPath, const QString &qrealSourceFilesPath
			, const QString &newMetamodelName
			, const QString &newEditorPath, const QString &relativeNewEditorPath
	);

	/// Returns dom document which contains metamodel
	QDomDocument loadMetamodelFromFile(const QString &metamodelPath);

	/// Returns dom document which contains all elements (do not forget to have
	/// one root element)
	QDomDocument loadElementsFromString(const QString &elementsXml);

	/// Saves converted metamodel in .xml file to specified path
	void saveMetamodelInFile(const QDomDocument &metamodel, const QString &metamodelPath);

	/// Inserts elements in <diagram></diagram> specified sublevel with creating
	/// this sublevel if it doesn't exist. For example graphicTypes and nonGraphicTypes.
	void insertElementsInDiagramSublevel(QDomDocument metamodel
			, const QString &sublevelName, QDomNodeList elements);

	/// Inserts one element
	void insertElementInDiagramSublevel(QDomDocument metamodel
			, const QString &sublevelName, const QDomElement &element);

	/// Appends all childrens to parent
	void appendElements(QDomNode parent, QDomNodeList children);

	QStringList collectAllGraphicTypesInMetamodel(const QDomDocument &metamodel
			, bool isDisplayedName) const;

	/// Appends types to specified child of element for creating container or smth
	void appendTypesToElement(
			QDomDocument parentDomDocument ///< Used for creating new elements
			, QDomElement parent, const QString &childName
			, const QString &prefix ///< Prefix of the whole name of element type like metamodel name
			, const QStringList &elementTypes ///< Element types w/o prefix
	);

	QDomElement diagramElement(const QDomDocument &metamodel) const;

private:

	/// Merge absolute path of file with relative from it
	QString mergePaths(const QString &begin, const QString &end) const;

	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};

}
