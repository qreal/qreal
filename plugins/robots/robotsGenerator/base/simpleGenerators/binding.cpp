#include "binding.h"

using namespace qReal::robots::generators::simple;

Binding::ConverterInterface::~ConverterInterface()
{
}

QString Binding::EmptyConverter::convert(QString const &data) const
{
	return data;
}

Binding::Binding(QString const &label, QString const &property)
	: mLabel(label), mProperty(property), mConverter(new EmptyConverter)
{
}

Binding::Binding(QString const &label, QString const &property
		, ConverterInterface const *converter)
	: mLabel(label), mProperty(property), mConverter(converter)
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
	data.replace(mLabel, mConverter->convert(property));
}
