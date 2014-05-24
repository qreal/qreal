#pragma once

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtGui/QIcon>
#include <QtCore/QPair>
#include "../../../../qrgui/editorPluginInterface/editorInterface.h"

class EditorPlugin : public QObject, public qReal::EditorInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::EditorInterface)
	Q_PLUGIN_METADATA(IID "Editor")

public:

	EditorPlugin();

	QString id() const { return "Editor"; }

	QStringList diagrams() const override;
	QStringList elements(QString const &diagram) const override;
	QStringList getPropertiesWithDefaultValues(QString const &element) const override;

	QStringList getTypesContainedBy(QString const &element) const override;
	QList<QPair<QPair<QString, QString>, QPair<bool, QString>>> getPossibleEdges(QString const &element) const override;
	QList<qReal::EditorInterface::ExplosionData> explosions(QString const &diagram, QString const &element) const override;

	int isNodeOrEdge(QString const &element) const override;

	qReal::ElementImpl* getGraphicalObject(QString const &diagram, QString const &element) const override;
	QString getPropertyType(QString const &element, QString const &property) const override;
	QString getPropertyDefaultValue(QString const &element, QString const &property) const override;
	QStringList getPropertyNames(QString const &diagram, QString const &element) const override;
	QStringList getPortTypes(QString const &diagram, QString const &element) const override;
	QStringList getReferenceProperties(QString const &diagram, QString const &element) const override;
	QStringList getEnumValues(QString name) const override;
	QString getGroupsXML() const override;
	QList<QPair<QString, QString>> getParentsOf(QString const &diagram, QString const &element) const override;

	QString editorName() const override;
	QString diagramName(QString const &diagram) const override;
	QString diagramNodeName(QString const &diagram) const override;
	QString elementName(QString const &diagram, QString const &element) const override;
	QString elementDescription(QString const &diagram, QString const &element) const override;
	QString propertyDescription(QString const &diagram, QString const &element, QString const &property) const override;
	QString propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const override;
	QString elementMouseGesture(QString const &digram, QString const &element) const override;

	QList<qReal::ListenerInterface*> listeners() const override;

	bool isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const override;

	QStringList diagramPaletteGroups(QString const &diagram) const override;
	QStringList diagramPaletteGroupList(QString const &diagram, QString const &group) const override;
	QString diagramPaletteGroupDescription(QString const &diagram, QString const &group) const override;
	bool shallPaletteBeSorted(QString const &diagram) const override;

private:
	virtual void initPlugin();
	virtual void initMouseGestureMap();
	virtual void initNameMap();
	virtual void initPropertyMap();
	virtual void initPropertyDefaultsMap();
	virtual void initDescriptionMap();
	virtual void initParentsMap();
	virtual void initPaletteGroupsMap();
	virtual void initPaletteGroupsDescriptionMap();
	virtual void initShallPaletteBeSortedMap();
	virtual void initExplosionsMap();

	QMap<QString, QIcon> mIconMap;
	QMap<QString, QString> mDiagramNameMap;
	QMap<QString, QString> mDiagramNodeNameMap;
	QMap<QString, QMap<QString, QString>> mPropertyTypes;
	QMap<QString, QMap<QString, QString>> mPropertyDefault;
	QMap<QString, QMap<QString, QString>> mElementsNameMap;
	QMap<QString, QMap<QString, QString>> mElementsDescriptionMap;
	QMap<QString, QMap<QString, QMap<QString, QString>>> mPropertiesDescriptionMap;
	QMap<QString, QMap<QString, QMap<QString, QString>>> mPropertiesDisplayedNamesMap;
	QMap<QString, QMap<QString, QString>> mElementMouseGesturesMap;
	QMap<QString, QMap<QString, QList<QPair<QString, QString>>>> mParentsMap;  // Maps diagram and element to a list of diagram-element pairs of parents (generalization relation).
	QMap<QString, QList<QPair<QString, QStringList>>> mPaletteGroupsMap;  // Maps element`s lists of all palette groups.
	QMap<QString, QMap<QString, QString>> mPaletteGroupsDescriptionMap;
	QMap<QString, bool> mShallPaletteBeSortedMap;
	QMap<QString, QMap<QString, QList<qReal::EditorInterface::ExplosionData>>> mExplosionsMap;
};

