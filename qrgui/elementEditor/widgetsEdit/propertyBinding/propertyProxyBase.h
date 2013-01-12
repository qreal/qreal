#pragma once

#include <QtCore/QVariant>

namespace qReal
{

class PropertyProxyBase : public QObject
{

public:
//	PropertyProxyBase();

	virtual void setPropertyValue(QString const &propertyName, QVariant const &value) = 0;
};

}
