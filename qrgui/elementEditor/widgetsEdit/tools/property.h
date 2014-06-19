#pragma once

#include <thirdparty/qtpropertybrowser/src/qtvariantproperty.h>
#include <QtCore/QObject>
#include <QtCore/QString>

namespace qReal
{
namespace widgetsEdit
{

class Property
{
public:
	Property(QString const &name = "", int type = 0
			, QtVariantProperty *qtProperty = NULL);

	QString name() const;
	void setName(QString const &name);

	int type() const;
	void setType(int type);

	QtVariantProperty *qtProperty() const;
	void setQtProperty(QtVariantProperty *qtProperty);

private:
	QString mName;
	int mType;
	QtVariantProperty *mQtProperty;
};

}
}
