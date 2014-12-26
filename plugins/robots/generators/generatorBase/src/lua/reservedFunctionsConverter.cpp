#include "reservedFunctionsConverter.h"

#include <QtCore/QStringList>

using namespace generatorBase::lua;

ReservedFunctionsConverter::ReservedFunctionsConverter(QString const &pathToTemplates)
	: TemplateParametrizedEntity(pathToTemplates)
{
}

QString ReservedFunctionsConverter::convert(QString const &name, QStringList const &args) const
{
	QStringList const oneArgumentFloatFunctions = { "sin", "cos", "ln", "exp", "asin", "acos", "atan"
			, "sgn", "sqrt", "abs", "ceil", "floor", "random" };
	int const index = oneArgumentFloatFunctions.indexOf(name);
	if (index >= 0) {
		return readTemplate(QString("functions/%1.t").arg(name)).replace("@@ARGUMENT@@"
				, args.count() ? args[0] : QString());
	}

	if (name == "time") {
		/// @todo: generate timestamps code in only when required
		return readTemplate("functions/time.t");
	}

	return QString();
}
