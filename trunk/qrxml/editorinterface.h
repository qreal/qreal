#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include <QtGui/QIcon>

// TODO: Выделить что-то вроде Plugins SDK - куски кода основной системы,
// которые должны использоваться при написании плагинов.
#include "../qrgui/umllib/uml_element.h"

class EditorInterface
{
public:
	virtual ~EditorInterface() {}

	virtual void initPlugin() = 0;
	virtual QString id() const = 0;

	virtual QStringList diagrams() const = 0;
	virtual QStringList elements(QString const &diagram) const = 0;

	virtual QIcon getIcon(QString const &diagram, QString const &element) const = 0;
	virtual UML::Element* getGraphicalObject(QString const &diagram, QString const &element) const = 0;

	virtual QString editorName() const = 0;
	virtual QString diagramName(QString const &diagram) const = 0;
	virtual QString elementName(QString const &diagram, QString const &element) const = 0;
};

Q_DECLARE_INTERFACE(EditorInterface, "ru.tepkom.QReal.EditorInterface/0.4")
