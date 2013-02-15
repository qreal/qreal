#include "metaPropertyBinding.h"

using namespace qReal;

MetaPropertyBinding::MetaPropertyBinding(QObject *proxy, QString const &source
		, QString const &target)
	: mProxy(proxy), mSource(source), mTarget(target)
{
	QMetaObject const *metaObject = proxy->metaObject();
	int const propertyCount = metaObject->propertyCount();
	for (int i = 0; i < propertyCount; ++i) {
		QMetaProperty const metaProperty = metaObject->property(i);
		if (metaProperty.isUser() && metaProperty.isDesignable()
				&& metaProperty.name() == mSource) {
			mMetaProperty = metaProperty;
			break;
		}
	}
}

QString MetaPropertyBinding::source() const
{
	return mSource;
}

QString MetaPropertyBinding::binding() const
{
	return mTarget;
}

void MetaPropertyBinding::setPropertyValue(QVariant const &value)
{
	mMetaProperty.write(mProxy, value);
}

void MetaPropertyBinding::setEnumValues(QStringList const &values)
{
	Q_UNUSED(values)
}
