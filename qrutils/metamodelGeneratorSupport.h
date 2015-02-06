#pragma once

#include "utilsDeclSpec.h"

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
	MetamodelGeneratorSupport(qReal::ErrorReporterInterface *errorReporter,
			qReal::gui::MainWindowInterpretersInterface *mainWindowInterface);
	~MetamodelGeneratorSupport();

	/// Compiles and loads to QReal new semantics editor plugin
	void loadPlugin(
			const QString &directoryName  ///< Directory where editor was generated (containing .pro file of that editor)
			, const QString &metamodelName  ///< Metamodel name as set in metamodel
			, const QString &pathToQmake  ///< qmake command
			, const QString &pathToMake  ///< make command
			, const QString &extension  ///< Extension of the resulting binary file with editor plugin (.dll/.so)
			, const QString &prefix  ///< Optional OS-dependent prefix for resulting binary file (lib for linux, for example)
	);
	
	/// Generate .pro file of new editor plugin
	void generateProFile(
			QDomDocument metamodel, ///< Metamodel is needed to fix 'include' .xml paths
			const QString &baseMetamodelPath, const QString &qrealSourceFilesPath,
			const QString &newMetamodelName,
			const QString &newEditorPath, const QString &relativeNewEditorPath
	);
	
	/// Returns dom document which contains metamodel
	QDomDocument loadMetamodelFromFile(const QString &metamodelPath);
	
	/// Returns dom document which contains all elements (do not forget to have
	/// one root element)
	QDomDocument loadElementsFromString(const QString &elementsXml);
	
	/// Saves converted metamodel in .xml file to specified path
	void saveMetamodelInFile(QDomDocument const &metamodel, const QString &metamodelPath);
	
	/// Inserts elements in <diagram></diagram> specified sublevel with creating
	/// this sublevel if it doesn't exist. For example graphicTypes and nonGraphicTypes.
	void insertElementsInDiagramSublevel(QDomDocument metamodel,
			const QString &sublevelName, QDomNodeList elements);
	
	/// Inserts one element
	void insertElementInDiagramSublevel(QDomDocument metamodel,
			const QString &sublevelName, QDomElement const &element);
	
	/// Appends all childrens to parent
	void appendElements(QDomNode parent, QDomNodeList children);
	
	QStringList collectAllGraphicTypesInMetamodel(QDomDocument const &metamodel,
			bool isDisplayedName) const;
	
	/// Appends types to specified child of element for creating container or smth
	void appendTypesToElement(
			QDomDocument parentDomDocument, ///< Used for creating new elements
			QDomElement parent, const QString &childName,
			const QString &prefix, ///< Prefix of the whole name of element type like metamodel name
			const QStringList &elementTypes ///< Element types w/o prefix
	);
	
	QDomElement diagramElement(QDomDocument const &metamodel) const;
	
private:
	
	/// Merge absolute path of file with relative from it
	QString mergePaths(const QString &begin, const QString &end) const;
	
	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};

}
