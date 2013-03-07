#pragma once

#include "../../editorPluginInterface/propertyEditorInterface.h"
#include "../../../qrutils/graphicsUtils/layoutHandlers/layoutHandlerFactory.h"

namespace qReal
{
namespace layouts
{

/// Implements PropertyEditorInterface for layout factory
/// to be pulled into ElementImpl for layout binding
class NodeElementLayoutFactory : public LayoutHandlerFactory
		, public PropertyEditorInterface
{
public:
	NodeElementLayoutFactory(QString const &bindingTarget
			, QGraphicsWidget *layoutHost);
	virtual ~NodeElementLayoutFactory();

	virtual QString binding() const;
	virtual void setPropertyValue(QVariant const &value);
	virtual void setEnumValues(QStringList const &values);

private:
	LayoutType stringToType(QString const &stringType);

	QString mBinding;
};

}
}
