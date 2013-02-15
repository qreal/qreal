#pragma once

#include "../../../umllib/nodeElement.h"
#include "../../../editorPluginInterface/propertyEditorInterface.h"

namespace qReal
{

class PropertyBinder : public PropertyEditorInterface
{
public:
	PropertyBinder();

	virtual QString binding() const;
	virtual void setPropertyValue(QVariant const &value);
	virtual void setEnumValues(QStringList const &values);
};

}
