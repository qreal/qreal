#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QString>
#include <QtGui/QIcon>

#include "editorPluginInterface/sdfRendererInterface.h"
#include "editorPluginInterface/elementImpl.h"
#include "editorPluginInterface/listenerInterface.h"

namespace qReal {

class EditorInterface
{
public:
	virtual ~EditorInterface() {}

	struct ExplosionData
	{
	public:
		ExplosionData(QString targetDiagram, QString targetElement
				, bool isReusable, bool requiresImmediateLinkage)
			: targetDiagram(targetDiagram)
			, targetElement(targetElement)
			, isReusable(isReusable)
			, requiresImmediateLinkage(requiresImmediateLinkage)
		{
		}

		QString targetDiagram;
		QString targetElement;
		bool isReusable;
		bool requiresImmediateLinkage;
	};

	virtual void initPlugin() = 0;
	virtual QString id() const = 0;

	virtual QStringList diagrams() const = 0;
	virtual QStringList elements(QString const &diagram) const = 0;

	virtual QStringList getPropertiesWithDefaultValues(QString const &element) const = 0;
	virtual QStringList getTypesContainedBy(QString const &element) const = 0;
	virtual QList<ExplosionData> explosions(QString const &diagram, QString const &element) const = 0;
	virtual QList<QPair<QPair<QString, QString>, QPair<bool, QString> > >
			getPossibleEdges(QString const &element) const = 0;

	virtual int isNodeOrEdge(QString const &element) const = 0;//(-1) means "edge", (+1) means "node"

	virtual QIcon getIcon(SdfIconEngineV2Interface *engine) const = 0;
	virtual ElementImpl* getGraphicalObject(QString const &diagram, QString const &element) const = 0;
	virtual QStringList getPropertyNames(QString const &diagram, QString const &element) const = 0;
	virtual QStringList getPortTypes(QString const &diagram, QString const &element) const = 0;
	virtual QStringList getReferenceProperties(QString const &diagram, QString const &element) const = 0;
	virtual QString getPropertyType(QString const &element, QString const &name) const = 0;
	virtual QString getPropertyDefaultValue(QString const &element, QString const &property) const = 0;
	virtual QStringList getEnumValues(QString name) const = 0;
	virtual QString getGroupsXML() const = 0;
	virtual QList<QPair<QString, QString> > getParentsOf(QString const &diagram, QString const &element) const = 0;

	virtual QString editorName() const = 0;
	virtual QString diagramName(QString const &diagram) const = 0;
	virtual QString diagramNodeName(QString const &diagram) const = 0;
	virtual QString elementName(QString const &diagram, QString const &element) const = 0;
	virtual QString elementMouseGesture(QString const &diagram, QString const &element) const = 0;
	virtual QString elementDescription(QString const &diagram, QString const &element) const = 0;
	virtual QString propertyDescription(QString const &diagram, QString const &element
			, QString const &property) const = 0;
	virtual QString propertyDisplayedName(QString const &diagram, QString const &element
			, QString const &property) const = 0;
	virtual QList<qReal::ListenerInterface*> listeners() const = 0;

	virtual bool isParentOf(QString const &parentDiagram, QString const &parentElement
			, QString const &childDiagram, QString const &childElement) const = 0;

	virtual QStringList diagramPaletteGroups(QString const &diagram) const = 0;
	virtual QStringList diagramPaletteGroupList(QString const &diagram, QString const &group) const = 0;
	virtual QString diagramPaletteGroupDescription(QString const &diagram, QString const &group) const = 0;
};

}

Q_DECLARE_INTERFACE(qReal::EditorInterface, "ru.tepkom.QReal.EditorInterface/0.9")
