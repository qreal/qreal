#pragma once

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtGui/QIcon>
#include <QPair>
#include "../../../../qrgui/editorPluginInterface/editorInterface.h"

class BlockDiagramMetamodelPlugin : public QObject, public qReal::EditorInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::EditorInterface)

public:

	BlockDiagramMetamodelPlugin();

	virtual void initPlugin();
	virtual void initMouseGestureMap();
	virtual void initNameMap();
	virtual void initPropertyMap();
	virtual void initPropertyDefaultsMap();
	virtual void initDescriptionMap();
	virtual void initParentsMap();
	virtual void initPaletteGroupsMap();

	virtual QString id() const { return "BlockDiagramMetamodel"; }

	virtual QStringList diagrams() const;
	virtual QStringList elements(QString const &diagram) const;
	virtual QStringList getPropertiesWithDefaultValues(QString const &element) const;

	virtual QStringList getTypesContainedBy(QString const &element) const;
	virtual QStringList getConnectedTypes(QString const &element) const;
	virtual QStringList getUsedTypes(QString const &element) const;
	virtual QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > getPossibleEdges(QString const &element) const;

	virtual int isNodeOrEdge(QString const &element) const; 

	virtual QIcon getIcon(SdfIconEngineV2Interface *engine) const;
	virtual ElementImpl* getGraphicalObject(QString const &diagram, QString const &element) const;
	virtual QString getPropertyType(QString const &element, QString const &property) const;
	virtual QString getPropertyDefaultValue(QString const &element, QString const &property) const;
	virtual QStringList getPropertyNames(QString const &diagram, QString const &element) const;
	virtual QStringList getEnumValues(QString name) const;
	virtual QList<QPair<QString, QString> > getParentsOf(QString const &diagram, QString const &element) const;

	virtual QString editorName() const;
	virtual QString diagramName(QString const &diagram) const;
	virtual QString diagramNodeName(QString const &diagram) const;
	virtual QString elementName(QString const &diagram, QString const &element) const;
	virtual QString elementDescription(QString const &diagram, QString const &element) const;
	virtual QString propertyDescription(QString const &diagram, QString const &element, QString const &property) const;
	virtual QString propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const;
	virtual QString elementMouseGesture(QString const &digram, QString const &element) const;

	virtual QList<qReal::ListenerInterface*> listeners() const;

	virtual bool isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const;

	virtual QStringList diagramPaletteGroups(QString const &diagram) const;
	virtual QStringList diagramPaletteGroupList(QString const &diagram, QString const &group) const;

private:
	QMap<QString, QIcon> iconMap;
	QMap<QString, QString> diagramNameMap;
	QMap<QString, QString> diagramNodeNameMap;
	QMap<QString, QMap<QString, QString> > propertyTypes;
	QMap<QString, QMap<QString, QString> > propertyDefault;
	QMap<QString, QMap<QString, QString> > elementsNameMap;
	QMap<QString, QMap<QString, QString> > elementsDescriptionMap;
	QMap<QString, QMap<QString, QMap<QString, QString> > > propertiesDescriptionMap;
	QMap<QString, QMap<QString, QMap<QString, QString> > > propertiesDisplayedNamesMap;
	QMap<QString, QMap<QString, QString> > elementMouseGesturesMap;
	QMap<QString, QMap<QString, QList<QPair<QString, QString> > > > parentsMap;  // Maps diagram and element to a list of diagram-element pairs of parents (generalization relation).
	QMap<QString, QMap<QString, QStringList > > paletteGroupsMap;  // Maps element`s lists of all palette groups.
};

