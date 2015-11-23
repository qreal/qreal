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
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace converters {

/// A base class for converters that generate template path from their conversion data.
/// Must provide path with @@DATA@@ label that will be replaced by data.
/// May be useful for some enums.
class ROBOTS_GENERATOR_EXPORT DynamicPathConverter : public TemplateParametrizedConverter
{
public:
	/// @param pathFromRoot A path from templates root containing placeholder for data
	/// (for example colors/@@PATH@@.t)
	DynamicPathConverter(const QStringList &pathsToTemplates, const QString &pathFromRoot);

	QString convert(const QString &data) const override;

private:
	const QString mPathFromRoot;
};

}
}
