#pragma once

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QStringList>
#include <QtCore/QPair>
#include <QtGui/QIcon>

#include "listenerManager.h"
#include "../../qrkernel/ids.h"
#include "../editorPluginInterface/editorInterface.h"
#include "../../qrrepo/graphicalRepoApi.h"
#include "../../qrrepo/logicalRepoApi.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrrepo/repoApi.h"
#include "editorManagerInterface.h"

class Element;

namespace qReal {

class InterpreterEditorManager : public QObject, public EditorManagerInterface
{
	Q_OBJECT

public:
	explicit InterpreterEditorManager(QString const &fileName, QObject *parent = NULL);

	IdList editors() const;
	IdList diagrams(Id const &editor) const;
	IdList elements(Id const &diagram) const;
	bool loadPlugin(QString const &pluginName);
	bool unloadPlugin(QString const &pluginName);

	QString mouseGesture(Id const &id) const;
	QString friendlyName(Id const &id) const;
	QString description(Id const &id) const;
	QString propertyDescription(Id const &id, QString const &propertyName) const;
	QString propertyDisplayedName(Id const &id, QString const &propertyName) const;
	QIcon icon(Id const &id) const;
	Element* graphicalObject(Id const &id) const;

	IdList containedTypes(const Id &id) const;
	IdList connectedTypes(const Id &id) const;
	IdList usedTypes(const Id &id) const;
	QStringList enumValues(Id const &id, const QString &name) const;
	QString typeName(Id const &id, const QString &name) const;
	QStringList allChildrenTypesOf(Id const &parent) const;

	bool isEditor(Id const &id) const;
	bool isDiagram(Id const &id) const;
	bool isElement(Id const &id) const;

	virtual QStringList propertyNames(Id const &id) const;
	virtual QString defaultPropertyValue(Id const &id, QString name) const;
	virtual QStringList propertiesWithDefaultValues(Id const &id) const;

	IdList checkNeededPlugins(qrRepo::LogicalRepoApi const &logicalApi
			, qrRepo::GraphicalRepoApi const &graphicalApi) const;
	bool hasElement(Id const &element) const;

	Id findElementByType(QString const &type) const;
	QList<ListenerInterface *> listeners() const;

	bool isDiagramNode(Id const &id) const;

	bool isParentOf(Id const &child, Id const &parent) const;
	bool isGraphicalElementNode(const Id &id) const;

	QList<StringPossibleEdge> possibleEdges(QString const &editor, QString const &element) const;
	QStringList elements(QString const &editor, QString const &diagram) const;
	int isNodeOrEdge(QString const &editor, QString const &element) const;
	bool isParentOf(QString const &editor, QString const &parentDiagram, QString const &parentElement
			, QString const &childDiagram, QString const &childElement) const;
	QString diagramName(QString const &editor, QString const &diagram) const;
	QString diagramNodeName(QString const &editor, QString const &diagram) const;
	bool isInterpretationMode() const;

	bool isParentProperty(Id const &id, QString const &propertyName) const;
	void deletePropertyInElement(qrRepo::RepoApi *repo, Id const &editor, Id const &diagram
			, QString const &propDisplayedName) const;
	void deleteProperty(QString const &propDisplayedName) const;
	void addProperty(Id const &id, QString const &propDisplayedName) const;
	void updateProperties(Id const &id, QString const &property, QString const &propertyType
			, QString const &propertyDefaultValue, QString const &propertyDisplayedName) const;
	QString propertyNameByDisplayedName(Id const &id, QString const &displayedPropertyName) const;
	IdList children(Id const &parent) const;
	QString shape(Id const &id) const;
	void updateShape(Id const &id, QString const &graphics) const;
	void deleteElement(qReal::MainWindow *mainWindow, Id const &id) const;
	bool isRootDiagramNode(Id const &id) const;
	void addNodeElement(Id const &diagram, QString const &name, bool isRootDiagramNode) const;
	void addEdgeElement(Id const &diagram, QString const &name, QString const &labelText, QString const &labelType
			, QString const &lineType, QString const &beginType, QString const &endType) const;
	QPair<Id, Id> createEditorAndDiagram(QString const &name) const;
	void saveMetamodel(QString const &newMetamodelFileName);
	QString saveMetamodelFilePath() const;
	//unsupported methods:
	QStringList paletteGroups(Id const &editor, Id const &diagram) const;
	QStringList paletteGroupList(Id const &editor,Id const &diagram, QString const &group) const;
	QString paletteGroupDescription(Id const &editor, const Id &diagram, const QString &group) const;
	virtual QStringList referenceProperties(Id const &id) const;
	IdList groups(Id const &diagram);
	Pattern getPatternByName (QString const &str) const;
	QList<QString> getPatternNames() const;
	QSize iconSize(Id const &id) const;

private:
	void setProperty(qrRepo::RepoApi* repo, Id const &id, QString const &property, QVariant const &propertyValue) const;
	Id element(Id const &id, qrRepo::RepoApi const * const repo, Id const &diagram) const;
	Id diagramOrElement(Id const &id, qrRepo::RepoApi const * const repo, Id const &editor) const;
	void setStandartConfigurations(qrRepo::RepoApi *repo, Id const &id, Id const &parent, const QString &name) const;
	QPair<qrRepo::RepoApi*, Id> repoAndMetaId(Id const &id) const;
	QPair<qrRepo::RepoApi*, Id> repoAndElement(QString const &editor, QString const &element) const;
	QPair<qrRepo::RepoApi*, Id> repoAndDiagram(QString const &editor, QString const &diagram) const;
	QPair<Id, Id> editorAndDiagram(qrRepo::RepoApi const * const repo, Id const &element) const;
	QMap<QString, qrRepo::RepoApi*> mEditorRepoApi;
	QString mMetamodelFile;

	class CheckPropertyForParent;
	class CompareProperty;
	class GetProperty;
	class HasProperty;

	QStringList propertiesFromParents(Id const &id, QString const &propertyName, CheckPropertyForParent const &checker) const;
	QString valueOfProperty(Id const &id, QString const &propertyName, QString const &value) const;
};

}
