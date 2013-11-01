#include "propertyProxyBase.h"
#include "metaPropertyBinding.h"

using namespace qReal;

PropertyProxyBase::PropertyProxyBase(QWidget *widget)
	: QObject(widget), mWidget(widget)
{
}

void PropertyProxyBase::onPropertyChanged(QString const &name
		, QVariant const &value)
{
	setValueInRepo(name, value);
	emit propertyChanged(name, value);
}

QMap<QString, QString> PropertyProxyBase::outerBindings() const
{
	return mOuterBindings;
}

void PropertyProxyBase::setOuterBindings(QMap<QString, QString> const &bindings)
{
	mOuterBindings = bindings;
}

void PropertyProxyBase::generateBinders(QList<PropertyEditorInterface *> &editors)
{
	foreach (QString const &source, mOuterBindings.keys()) {
		foreach(QString const &target, mOuterBindings.values(source)) {
			editors << new MetaPropertyBinding(this, source, target);
		}
	}
}

NodeElement *PropertyProxyBase::getNode() const
{
	QWidget *root = mWidget;
	while (root && !root->graphicsProxyWidget()) {
		root = root->parentWidget();
	}
	return root ? dynamic_cast<NodeElement *>(root->graphicsProxyWidget()) : NULL;
}

void PropertyProxyBase::setValueInRepo(QString const &property, QVariant const &value)
{
	NodeElement *node = getNode();
	if (node) {
		foreach (QString const &target, mOuterBindings.values(property)) {
			node->setLogicalProperty(target, value.toString());
		}
	}
}
