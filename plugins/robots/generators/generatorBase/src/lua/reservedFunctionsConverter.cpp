/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
			, "sgn", "sqrt", "abs", "ceil", "floor", "random", "print" };
	if (oneArgumentFloatFunctions.contains(name)) {
		return readTemplate(QString("functions/%1.t").arg(name)).replace("@@ARGUMENT@@"
				, args.count() ? args[0] : QString());
	}

	const QStringList twoArgumentsFloatFunctions = { "min", "max" };
	if (twoArgumentsFloatFunctions.contains(name)) {
		return readTemplate(QString("functions/%1.t").arg(name))
				.replace("@@ARGUMENT1@@", args.count() ? args[0] : QString())
				.replace("@@ARGUMENT2@@", args.count() >= 2 ? args[1] : QString());
	}

	if (name == "time") {
		/// @todo: generate timestamps code in only when required
		return readTemplate("functions/time.t");
	}

	if (name == "sensor") {
		/// @todo: Display an error if wrong arguments count
		return readTemplate("sensors/scalar.t").replace("@@PORT@@", args.count() ? args[0] : QString());
	}

	if (name == "vectorSensor") {
		/// @todo: Display an error if wrong arguments count
		return readTemplate("sensors/vector.t").replace("@@PORT@@", args.count() ? args[0] : QString());
	}

	return QString();
}
