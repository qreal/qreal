#include "reservedFunctionsConverter.h"

#include <QtCore/QStringList>

using namespace generatorBase::lua;

ReservedFunctionsConverter::ReservedFunctionsConverter(const QString &pathToTemplates)
	: TemplateParametrizedEntity(pathToTemplates)
{
}

QString ReservedFunctionsConverter::convert(const QString &name, const QStringList &args) const
{
	const QStringList oneArgumentFloatFunctions = { "sin", "cos", "ln", "exp", "asin", "acos", "atan"
			, "sgn", "sqrt", "abs", "ceil", "floor", "random" };
	const int index = oneArgumentFloatFunctions.indexOf(name);
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
