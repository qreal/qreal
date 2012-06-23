#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QHash>

#include "../../../qrkernel/ids.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../../qrrepo/logicalRepoApi.h"

namespace metaEditor {

/// Generates metamodel in .xml format to be compiled into C++ sources by qrxc tool
class EditorGenerator
{
public:
	explicit EditorGenerator(qrRepo::LogicalRepoApi const &api, qReal::ErrorReporterInterface &errorReporter);

	QHash<qReal::Id, QPair<QString, QString> > getMetamodelList();
	QString generateEditor(qReal::Id const &metamodelId, QString const &pathToFile, QString const &pathToQRealSource);

private:
	void serializeObjects(QDomElement &parent, qReal::Id const &idParent);
	void createImport(QDomElement &parent, qReal::Id const &id);
	void createNode(QDomElement &parent, qReal::Id const &id);
	void createEdge(QDomElement &parent, qReal::Id const &id);
	void createEnum(QDomElement &parent, qReal::Id const &id);
	void setProperties(QDomElement &parent, qReal::Id const &id);
	void setContextMenuFields(QDomElement &parent, qReal::Id const &id);
	void setValues(QDomElement &parent, qReal::Id const &id);
	void setAssociations(QDomElement &parent, qReal::Id const &id);
	void setUsages(QDomElement &parent, qReal::Id const &id);
	void setConnections(QDomElement &parent, qReal::Id const &id);
	void setGeneralization(QDomElement &parent, qReal::Id const &id);
	void setPossibleEdges(QDomElement &parent, qReal::Id const &id);
	void setPin(QDomElement &parent, qReal::Id const &id);
	void setStatusElement(QDomElement &parent, qReal::Id const &id, QString const &tagName, QString const &propertyName);
	void setAction(QDomElement &parent, qReal::Id const &id);
	void createDiagrams(QDomElement &parent, qReal::Id const &id);
	void setContainer(QDomElement &parent, qReal::Id const &id);
	void setContainerProperties(QDomElement &parent, qReal::Id const &id);
	void newSetConnections(QDomElement &parent, const qReal::Id &id,
			QString const &commonTagName, QString const &internalTagName, QString const &typeName);
	void ensureCorrectness(qReal::Id const &id, QDomElement element, QString const &tagName, QString const &value);
	void setBoolValuesForContainer(QString const &propertyName, QDomElement &properties, qReal::Id const &id);
	void setSizesForContainer(QString const &propertyName, QDomElement &properties, qReal::Id const &id);
	static void copyImages(QString const &pathToFile);
	static QString calculateEditorPath(QString const &pathToFile, QString const &pathToQRealSource);

	qrRepo::LogicalRepoApi const &mApi;
	QDomDocument mDocument;
	QString mErrorText;
	qReal::IdList mElements;
	QString mDiagramName;
	qReal::ErrorReporterInterface &mErrorReporter;
};

}
