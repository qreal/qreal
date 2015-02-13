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
			, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName);
	~NxtGeneratorFactory() override;

	/// Returns an entity processing everything about BMP images that will be embedded into an executable.
	parts::Images &images();

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QString pathToTemplates() const override;

	/// Produces converter for color specification without taking ownership on it
	generatorBase::simple::Binding::ConverterInterface *colorConverter() const;

private:
	const QString mGeneratorName;
	parts::Images mImages;
};

}
