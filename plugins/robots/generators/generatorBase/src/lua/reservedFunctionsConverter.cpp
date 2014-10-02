#include "reservedFunctionsConverter.h"

#include <QtCore/QStringList>

using namespace generatorBase::lua;

ReservedFunctionsConverter::ReservedFunctionsConverter(QString const &pathToTemplates)
	: TemplateParametrizedEntity(pathToTemplates)
{
}

QString ReservedFunctionsConverter::convert(QString const &name, QStringList const &args) const
{
	if (name == "random") {
		if (args.count() != 1) {
			/// @todo: report error.
			return QString();
		}

		return readTemplate("functions/random.t").replace("@@UPPER_BOUND@@", args[0]);
	}

	/// @todo: add all supported functions here.

	return QString();
}
