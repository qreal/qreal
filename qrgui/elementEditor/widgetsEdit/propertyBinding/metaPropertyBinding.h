#pragma once

#include <QtCore/QMetaProperty>

#include <qrgui/editorPluginInterface/propertyEditorInterface.h>

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
	QString binding() const override;
	void setPropertyValue(QVariant const &value) override;
	void setEnumValues(QList<QPair<QString, QString>> const &values) override;

private:
	QObject *mProxy;
	QString mSource;
	QString mTarget;
	QMetaProperty mMetaProperty;
	bool mEnumEditor;
	QMap<QString, int> mQRealWtfEnumValuesMap;
};

}
