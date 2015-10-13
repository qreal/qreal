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

#pragma once

#include "generatorBase/converters/templateParametrizedConverter.h"

namespace generatorBase {
namespace converters {

/// Accepts an expression and a set of values and returns composite condition
/// of the equality of the given expression value to one of the given.
/// For example it can be done with a number of disjunctions or empty case expressions.
class SwitchConditionsMerger : public TemplateParametrizedConverter
{
public:
	SwitchConditionsMerger(const QStringList &pathsToTemplates
			, const ConverterInterface * const systemVariablesConverter
			, const QStringList &values);

	~SwitchConditionsMerger() override;

	QString convert(const QString &expression) const override;

private:
	const ConverterInterface *mSystemVariablesConverter;  // Takes ownership
	const QStringList mValues;
};

}
}
