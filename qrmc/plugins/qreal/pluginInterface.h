#pragma once

#include "../../../qrgui/editorPluginInterface/elementImpl.h"
#include "../../../qrgui/editorPluginInterface/sdfRendererInterface.h"
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtGui/QIcon>
#include <QPair>
#include "../../../qrgui/editorPluginInterface/editorInterface.h"

class QrealPlugin : public QObject, public qReal::EditorInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::EditorInterface)
        Q_PLUGIN_METADATA(IID "RobotsMetamodel")

public:

	QrealPlugin();

	virtual void initPlugin();
	virtual void initMouseGestureMap();
	virtual void initNameMap();
	virtual void initPropertyMap();
	virtual void initPropertyDefaultsMap();
	virtual void initParentsMap();

	virtual QString id() const { return "Qreal"; }

//temporary fixing
	QString version() const { return "1.0.0"; };

	virtual QStringList diagrams() const;
	virtual QStringList elements(QString const &diagram) const;
	virtual QString elementDescription(QString const &diagram, QString const &element) const;
	virtual QString propertyDescription(QString const &diagram, QString const &element, QString const &property) const;
	virtual QString propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const;
	virtual bool isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const;
	virtual QList<QPair<QString, QString> > getParentsOf(QString const &diagram, QString const &element) const;
	virtual QStringList diagramPaletteGroups(QString const &diagram) const;
	virtual QStringList diagramPaletteGroupList(QString const &diagram, QString const &group) const;
	virtual QString diagramPaletteGroupDescription(QString const &diagram, QString const &group) const;
	virtual QStringList getPropertiesWithDefaultValues(QString const &element) const;

	virtual QStringList getTypesContainedBy(QString const &element) const;
	virtual QStringList getConnectedTypes(QString const &element) const;
	virtual QStringList getUsedTypes(QString const &element) const;
	virtual QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > getPossibleEdges(QString const &element) const;

	virtual int isNodeOrEdge(QString const &element) const; 

//	virtual QIcon getIcon(qReal::SdfIconEngineV2Interface *engine) const;
	virtual qReal::ElementImpl* getGraphicalObject(QString const &diagram, QString const &element) const;
	virtual QString getPropertyType(QString const &element, QString const &property) const;
	virtual QString getPropertyDefaultValue(QString const &element, QString const &property) const;
	virtual QStringList getPropertyNames(QString const &diagram, QString const &element) const;
	virtual QStringList getReferenceProperties(QString const &/*diagram*/, QString const &element) const;
//	virtual QStringList getEnumValues(QString name) const;

	virtual QString editorName() const;
	virtual QString diagramName(QString const &diagram) const;
	virtual QString diagramNodeName(QString const &diagram) const;
	virtual QString elementName(QString const &diagram, QString const &element) const;
	virtual QString elementMouseGesture(QString const &digram, QString const &element) const;

	virtual QList<qReal::ListenerInterface*> listeners() const;




	QList<qReal::EditorInterface::ExplosionData> explosions(QString const &diagram, QString const &element) const override;
        QStringList getPortTypes(QString const &diagram, QString const &element) const override;
        QList<QPair<QString, QString>> getEnumValues(QString const &name) const override;
	bool isEnumEditable(QString const &name) const override;
        QString getGroupsXML() const override;
	bool shallPaletteBeSorted(QString const &diagram) const;	






private:
	QMap<QString, QIcon> iconMap;
	QMap<QString, QString> diagramNameMap;
	QMap<QString, QString> diagramNodeNameMap;
	QMap<QString, QMap<QString, QString> > propertyTypes;
	QMap<QString, QMap<QString, QString> > propertyDefault;
	QMap<QString, QMap<QString, QString> > elementsNameMap;
	QMap<QString, QMap<QString, QString> > elementMouseGesturesMap;
	QMap<QString, QMap<QString, QString> > elementsDescriptionMap;
	QMap<QString, QMap<QString, QMap<QString, QString> > > propertiesDescriptionMap;
	QMap<QString, QMap<QString, QMap<QString, QString> > > propertiesDisplayedNamesMap;
	QMap<QString, QMap<QString, QList<QPair<QString, QString> > > > parentsMap;
	QMap<QString, QMap<QString, QStringList > > paletteGroupsMap;
	QMap<QString, QMap<QString, QString > > paletteGroupsDescriptionMap;


	QMap<QString, QMap<QString, QList<qReal::EditorInterface::ExplosionData>>> mExplosionsMap;
	QMap<QString, bool> mShallPaletteBeSortedMap;


};

