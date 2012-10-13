#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtXml/QDomElement>

#include "property.h"
#include "tool.h"
#include "../../qtpropertybrowser/src/QtVariantPropertyManager"

namespace qReal
{
namespace widgetsEdit
{

class Tool;

class PropertyManager : public QObject
{
	Q_OBJECT

public:
	explicit PropertyManager(Tool *tool);

	QListIterator<Property *> userPropertiesIterator() const;
	QListIterator<Property *> allPropertiesIterator() const;
	QtVariantPropertyManager *qtPropertyManager() const;
	void onSelect();

	void generateXml(QDomElement &element, QDomDocument &document);
	void deserializeProperty(QDomElement const &element);

public slots:
	void changeProperty(QString const &name, QVariant const &value);

private slots:
	void propertyChanged(QtProperty *property, QVariant const &value);

private:
	void initProperties();
	QVariant propertyValue(QString const &name);
	void onPropertyChanged(QString const &name, QVariant const &value);
	Property *propertyFromMetaProperty(QMetaProperty const &metaProperty);
	Property *findProperty(QString const &name);
	int metaPropertyIndex(QString const &name);

	Tool *mTool;
	QtVariantPropertyManager *mManager;
	QList<Property *> mDesignableProperties;
	QMap<Property *, int> mUserProperties;

	QMap<QString, QList<int> > mEnumProperties;
	QSet<QString> mFlagProperties;
	bool mDeserializing;
};

}
}
