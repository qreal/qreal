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

#include <generatorBase/generatorFactoryBase.h>

#include "nxtGeneratorBase/nxtGeneratorBaseDeclSpec.h"
#include "parts/images.h"

namespace nxt {

class ROBOTS_NXT_GENERATOR_BASE_EXPORT NxtGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	NxtGeneratorFactory(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName);
	~NxtGeneratorFactory() override;

	/// Returns an entity processing everything about BMP images that will be embedded into an executable.
	parts::Images &images();

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QStringList pathsToTemplates() const override;

	/// Produces converter for color specification without taking ownership on it
	generatorBase::simple::Binding::ConverterInterface *colorConverter() const;

private:
	const QString mGeneratorName;
	parts::Images mImages;
};

}
