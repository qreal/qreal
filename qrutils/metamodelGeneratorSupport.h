#pragma once

#include "utilsDeclSpec.h"

#include "../qrgui/mainwindow/errorReporter.h"
#include "../qrgui/mainwindow/mainWindowInterpretersInterface.h"

namespace utils {

/// This class helps with generation new metamodels based on existing ones. For
/// example copying entire metamodel, insert some new elements, create container
/// or smth with all types in metamodel. Also it helps with generating .pro file
/// and building and loading new plugin.
class QRUTILS_EXPORT MetamodelGeneratorSupport : public QObject
{
	Q_OBJECT

public:
	MetamodelGeneratorSupport(qReal::ErrorReporterInterface *errorReporter,
			qReal::gui::MainWindowInterpretersInterface *mainWindowInterface);
	~MetamodelGeneratorSupport();

	/// Compiles and loads to QReal new semantics editor plugin
	void loadPlugin(
			QString const &directoryName  ///< Directory where editor was generated (containing .pro file of that editor)
			, QString const &metamodelName  ///< Metamodel name as set in metamodel
			, QString const &pathToQmake  ///< qmake command
			, QString const &pathToMake  ///< make command
			, QString const &extension  ///< Extension of the resulting binary file with editor plugin (.dll/.so)
			, QString const &prefix  ///< Optional OS-dependent prefix for resulting binary file (lib for linux, for example)
	);
	
	/// Generate .pro file of new editor plugin
	void generateProFile(
			QDomDocument metamodel, ///< Metamodel is needed to fix 'include' .xml paths
			QString const &baseMetamodelPath, QString const &qrealSourceFilesPath,
			QString const &newMetamodelName,
			QString const &newEditorPath, QString const &relativeNewEditorPath
	);
	
	/// Returns dom document which contains metamodel
	QDomDocument loadMetamodelFromFile(QString const &metamodelPath);
	
	/// Returns dom document which contains all elements (do not forget to have
	/// one root element)
	QDomDocument loadElementsFromString(QString const &elementsXml);
	
	/// Saves converted metamodel in .xml file to specified path
	void saveMetamodelInFile(QDomDocument const &metamodel, QString const &metamodelPath);
	
	/// Inserts elements in <diagram></diagram> specified sublevel with creating
	/// this sublevel if it doesn't exist. For example graphicTypes and nonGraphicTypes.
	void insertElementsInDiagramSublevel(QDomDocument metamodel,
			QString const &sublevelName, QDomNodeList elements);
	
	/// Inserts one element
	void insertElementInDiagramSublevel(QDomDocument metamodel,
			QString const &sublevelName, QDomElement const &element);
	
	/// Appends all childrens to parent
	void appendElements(QDomNode parent, QDomNodeList children);
	
	QStringList collectAllGraphicTypesInMetamodel(QDomDocument const &metamodel,
			bool isDisplayedName) const;
	
	/// Appends types to specified child of element for creating container or smth
	void appendTypesToElement(
			QDomDocument parentDomDocument, ///< Used for creating new elements
			QDomElement parent, QString const &childName,
			QString const &prefix, ///< Prefix of the whole name of element type like metamodel name
			QStringList const &elementTypes ///< Element types w/o prefix
	);
	
	QDomElement diagramElement(QDomDocument const &metamodel) const;
	
private:
	
	/// Merge absolute path of file with relative from it
	QString mergePaths(QString const &begin, QString const &end) const;
	
	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};

}
