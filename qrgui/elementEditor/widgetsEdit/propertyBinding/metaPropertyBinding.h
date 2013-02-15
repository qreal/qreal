#pragma once

#include <QtCore/QMetaProperty>

#include "../../../editorPluginInterface/propertyEditorInterface.h"

namespace qReal
{

// Best phrase ever %)
/// Qt reflection-based plugin property editors interaction implemetation
class MetaPropertyBinding : public PropertyEditorInterface
{
public:
	/// Creates two-way binding between @param source and @param target properties
	/// @param proxy An object with @param source property
	/// @param source The name of the shape editor or widgets editor property
	/// @param target The name of the QReal editor property
	MetaPropertyBinding(QObject *proxy, QString const &source, QString const &target);

	QString source() const;
	virtual QString binding() const;
	virtual void setPropertyValue(QVariant const &value);
	virtual void setEnumValues(QStringList const &values);

private:
	QObject *mProxy;
	QString mSource;
	QString mTarget;
	QMetaProperty mMetaProperty;
};

}
