#ifndef HANDMADE_H
#define HANDMADE_H

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtGui/QIcon>

#include "editorinterface.h"

class HandmadePlugin : public QObject, public EditorInterface
{
	Q_OBJECT
	Q_INTERFACES(EditorInterface)

public:

	HandmadePlugin();

	void initPlugin();

	QString id() const { return "TestEditor"; };

	QStringList diagrams() const;
	QStringList elements(QString diagram) const;

	QIcon getIcon(QString element) const;

	QString editorName() const;
	QString diagramName(QString diagram) const;
	QString elementName(QString diagram, QString element) const;

private:
	QMap<QString, QIcon> iconMap;
	
	QMap<QString, QString> diagramNameMap;
	QMap<QString, QMap<QString, QString> > elementsNameMap;
};

#endif
