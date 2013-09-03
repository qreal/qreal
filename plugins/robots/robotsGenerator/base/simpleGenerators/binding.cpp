#include "binding.h"

using namespace qReal::robots::generators::simple;

Binding::ConverterInterface::~ConverterInterface()
{
}

Binding::MultiConverterInterface::~MultiConverterInterface()
{
}

QString Binding::EmptyConverter::convert(QString const &data) const
{
	return data;
}

Binding *Binding::createDirect(QString const &label, QString const &property)
{
	return new Binding(label, property);
}

Binding *Binding::createConverting(QString const &label, QString const &property
		, ConverterInterface const *converter)
{
	return new Binding(label, property, converter);
}

Binding *Binding::createMultiTarget(QString const &label, QString const &property
		, MultiConverterInterface const *converter)
{
	return new Binding(label, property, converter);
}

Binding::Binding(QString const &label, QString const &property)
	: mLabel(label), mProperty(property), mConverter(new EmptyConverter), mMultiConverter(NULL)
{
}

Binding::Binding(QString const &label, QString const &property
		, ConverterInterface const *converter)
	: mLabel(label), mProperty(property), mConverter(converter), mMultiConverter(NULL)
{
}

Binding::Binding(QString const &label, QString const &property
		, MultiConverterInterface const *converter)
	: mLabel(label), mProperty(property), mConverter(NULL), mMultiConverter(converter)
{
}

Binding::~Binding()
{
	delete mConverter;
}

void Binding::apply(LogicalModelAssistInterface const &model
		, Id const &id, QString &data)
{
	QString const property = mProperty == "name"
			? model.name(id)
			: model.propertyByRoleName(id, mProperty).toString();

	if (mConverter) {
		data.replace(mLabel, mConverter->convert(property));
	} else {
		applyMulti(property, data);
	}
}

void Binding::applyMulti(QString const &property, QString &data)
{
	QString result;
	QStringList const parts = mMultiConverter->convert(property);
	foreach (QString const &part, parts) {
		QString dataCopy = data;
		dataCopy.replace(mLabel, part);
		result += dataCopy;
	}

	data = result;
}
