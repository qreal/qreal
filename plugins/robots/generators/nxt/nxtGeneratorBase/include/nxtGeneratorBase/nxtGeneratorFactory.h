#pragma once

#include <generatorBase/generatorFactoryBase.h>

#include "nxtGeneratorBase/nxtGeneratorBaseDeclSpec.h"
#include "parts/images.h"

namespace nxt {

class ROBOTS_NXT_GENERATOR_BASE_EXPORT NxtGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	NxtGeneratorFactory(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, QString const &generatorName);
	~NxtGeneratorFactory() override;

	/// Returns an entity processing everything about BMP images that will be embedded into an executable.
	parts::Images &images();

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(qReal::Id const &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QString pathToTemplates() const override;

	generatorBase::simple::Binding::ConverterInterface *stringPropertyConverter() const override;

	/// Produces converter for color specification without taking ownership on it
	generatorBase::simple::Binding::ConverterInterface *colorConverter() const;

private:
	QString const mGeneratorName;
	parts::Images mImages;
};

}
