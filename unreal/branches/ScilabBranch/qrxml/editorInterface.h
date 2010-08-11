#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QString>

#include <QtGui/QIcon>

// TODO: Выделить что-то вроде Plugins SDK - куски кода основной системы,
// которые должны использоваться при написании плагинов.
//#include "../qrgui/umllib/uml_element.h"
#include "../qrgui/umllib/sdfRendererInterface.h"
#include "../qrgui/umllib/elementImpl.h"
#include "../qrgui/editorManager/listenerInterface.h"

namespace qReal {

		class EditorInterface
		{
		public:
			virtual ~EditorInterface() {}

			virtual void initPlugin() = 0;
			virtual QString id() const = 0;

			virtual QStringList diagrams() const = 0;
			virtual QStringList elements(QString const &diagram) const = 0;

			virtual QStringList getPropertiesWithDefaultValues(QString const &element) const = 0;
			virtual QStringList getTypesContainedBy(QString const &element) const = 0;
			virtual QStringList getConnectedTypes(QString const &element) const = 0;
                        virtual QStringList getPortNames(QString const &element) const = 0;
			virtual QStringList getUsedTypes(QString const &element) const = 0;
			virtual QList<QPair<QPair<QString, QString>, QPair<bool, QString> > > getPossibleEdges(QString const &element) const = 0;

			virtual int isNodeOrEdge(QString const &element) const = 0;

			virtual QIcon getIcon(SdfIconEngineV2Interface *engine) const = 0;
			virtual UML::ElementImpl* getGraphicalObject(QString const &diagram, QString const &element) const = 0;
			virtual QStringList getPropertyNames(QString const &diagram, QString const &element) const = 0;
			virtual QString getPropertyType(QString const &element, QString const &name) const = 0;
			virtual QString getPropertyDefaultValue(QString const &element, QString const &property) const = 0;
			virtual QStringList getEnumValues(QString name) const = 0;

			virtual QString editorName() const = 0;
			virtual QString diagramName(QString const &diagram) const = 0;
			virtual QString diagramNodeName(QString const &diagram) const = 0;
			virtual QString elementName(QString const &diagram, QString const &element) const = 0;
			virtual QString elementMouseGesture(QString const &diagram, QString const &element) const = 0;

			virtual QList<qReal::ListenerInterface*> listeners() const = 0;
		};
}

Q_DECLARE_INTERFACE(qReal::EditorInterface, "ru.tepkom.QReal.EditorInterface/0.8")
