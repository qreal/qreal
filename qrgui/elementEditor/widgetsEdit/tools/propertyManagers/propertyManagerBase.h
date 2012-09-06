#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtXml/QDomElement>

#include "property.h"
#include "../tool.h"
#include "../../qtpropertybrowser/src/QtVariantPropertyManager"

namespace qReal
{
namespace widgetsEdit
{

class Tool;

class PropertyManagerBase : public QObject
{
	Q_OBJECT

public:
	explicit PropertyManagerBase(Tool *tool);

	QListIterator<Property *> propertiesIterator() const;
	QtVariantPropertyManager *qtPropertyManager() const;
	void onSelect();

	void generateXml(QDomElement &element, QDomDocument &document);
	void deserializeProperty(QDomElement const &element);

public slots:
	void changeProperty(QString const &name, QVariant const &value);

protected:
	void addProperty(QString const &name, int type);
	void addEnumProperty(QString const &name, QStringList const &types);
	virtual void onPropertyChanged(QString const &name, QVariant const &value);
	virtual QVariant propertyValue(QString const &name) = 0;

	Tool *mTool;

private slots:
	void propertyChanged(QtProperty *property, QVariant const &value);

private:
	Property *findProperty(QString const &name);

	QtVariantPropertyManager *mManager;
	QList<Property *> mProperties;

};

}
}
