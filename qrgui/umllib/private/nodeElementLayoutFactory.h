#pragma once

#include <editorPluginInterface/elementImpl.h>
#include <editorPluginInterface/propertyEditorInterface.h>
#include <qrutils/graphicsUtils/layoutHandlers/layoutHandlerFactory.h>

namespace qReal
{
namespace layouts
{

/// Implements PropertyEditorInterface for layout factory
/// to be pulled into ElementImpl for layout binding.
/// Also converts some enviriment actions into layout
/// framework ones
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

	void configure(ElementImpl * const impl);

	void processBeforeFirstPlacing(QGraphicsItem *element);

private:
	/// Padding that reserves space for title.
	static int const titlePadding = 25;
	/// Space between children inside sorting containers.
	static int const childSpacing = 10;

	LayoutType stringToType(QString const &stringType);

	int upperMargin(int const baseMargin) const;

	QString mBinding;
	QVector<int> mForestallingSize;
};

}
}
