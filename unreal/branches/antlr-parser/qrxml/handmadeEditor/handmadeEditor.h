#ifndef HANDMADE_H
#define HANDMADE_H

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtGui/QIcon>

#include "editorInterface.h"

class HandmadePlugin : public QObject, public qReal::EditorInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::EditorInterface)

public:

	HandmadePlugin();

	virtual void initPlugin();

	virtual QString id() const { return "TestEditor"; }

	virtual QStringList diagrams() const;
	virtual QStringList elements(QString const &diagram) const;

	virtual QIcon getIcon(QString const &diagram, QString const &element) const;
	virtual UML::Element* getGraphicalObject(QString const &diagram, QString const &element) const;
	virtual QStringList getPropertyNames(QString const &diagram, QString const &element) const;

	virtual QString editorName() const;
	virtual QString diagramName(QString const &diagram) const;
	virtual QString elementName(QString const &diagram, QString const &element) const;

private:
	QMap<QString, QIcon> iconMap;

	QMap<QString, QString> diagramNameMap;
	QMap<QString, QMap<QString, QString> > elementsNameMap;
};

#endif
