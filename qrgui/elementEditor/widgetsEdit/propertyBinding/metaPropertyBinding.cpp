#include <QtCore/QStringList>

#include "metaPropertyBinding.h"

using namespace qReal;

MetaPropertyBinding::MetaPropertyBinding(QObject *proxy, QString const &source
		, QString const &target)
	: mProxy(proxy), mSource(source), mTarget(target), mEnumEditor(false)
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
	if (mEnumEditor) {
		mMetaProperty.write(mProxy, mQRealWtfEnumValuesMap[value.toString()]);
	} else {
		mMetaProperty.write(mProxy, value);
	}
}

void MetaPropertyBinding::setEnumValues(QList<QPair<QString, QString>> const &values)
{
	// Here binding gets enum names from QReal engine side.
	// The principle of consistency works here: if we get enum
	// values from QReal, but on wtf side we have non-enum editor,
	// the only thing we can say is "I hope you know what you do dude".

	if (!mMetaProperty.isEnumType()) {
		return;
	}
	mEnumEditor = true;
	QMetaEnum const wtfMetaEnum = mMetaProperty.enumerator();
	int const wtfEnumValuesCount = wtfMetaEnum.keyCount();
	for (int i = 0; i < values.count(); ++i) {
		int const wtfEnumIndex = qMin(i, wtfEnumValuesCount - 1);
		mQRealWtfEnumValuesMap.insert(values[i].first, wtfMetaEnum.value(wtfEnumIndex));
	}
}
