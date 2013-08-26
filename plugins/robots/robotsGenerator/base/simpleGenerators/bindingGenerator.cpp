#include "bindingGenerator.h"

using namespace qReal::robots::generators::simple;

BindingGenerator::BindingGenerator()
{
}

BindingGenerator::ConverterInterface::~ConverterInterface()
{
}

QString BindingGenerator::EmptyConverter::convert(QString const &data)
{
	return data;
}
