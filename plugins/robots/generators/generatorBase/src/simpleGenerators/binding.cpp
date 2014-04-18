#include "generatorBase/simpleGenerators/binding.h"

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

Binding *Binding::createStatic(QString const &label, QString const &value)
{
	return new Binding(label, value, false);
}

Binding *Binding::createStaticConverting(QString const &label, QString const &value
		, ConverterInterface const *converter)
{
	return new Binding(label, value, false, converter);
}

Binding *Binding::createDirect(QString const &label, QString const &property)
{
	return new Binding(label, property, true);
}

Binding *Binding::createConverting(QString const &label, QString const &property
		, ConverterInterface const *converter)
{
	return new Binding(label, property, true, converter);
}

Binding *Binding::createMultiTarget(QString const &label, QString const &property
		, MultiConverterInterface const *converter)
{
	return new Binding(label, property, converter);
}

Binding::Binding(QString const &label, QString const &propertyOrValue, bool takeFromRepo)
	: mLabel(label)
	, mProperty(takeFromRepo ? propertyOrValue : "")
	, mValue(takeFromRepo ? "" : propertyOrValue)
	, mConverter(new EmptyConverter)
	, mMultiConverter(NULL)
{
}

Binding::Binding(QString const &label, QString const &propertyOrValue, bool takeFromRepo
		, ConverterInterface const *converter)
	: mLabel(label)
	, mProperty(takeFromRepo ? propertyOrValue : "")
	, mValue(takeFromRepo ? "" : propertyOrValue)
	, mConverter(converter)
	, mMultiConverter(NULL)
{
}

Binding::Binding(QString const &label, QString const &property
		, MultiConverterInterface const *converter)
	: mLabel(label)
	, mProperty(property)
	, mConverter(NULL)
	, mMultiConverter(converter)
{
}

Binding::~Binding()
{
	delete mConverter;
}

void Binding::apply(qrRepo::RepoApi const &repo
		, Id const &id, QString &data)
{
	QString const property = mProperty.isEmpty()
			? mValue
			: mProperty == "name"
					? repo.name(id)
					: repo.property(id, mProperty).toString();

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
