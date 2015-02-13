#pragma once

#include "generatorBase/templateParametrizedEntity.h"

namespace generatorBase {
namespace parts {

/// A base for entity that inspects some robot part usage and generates code
/// for its correct initialization, deinitialization and work
class InitTerminateCodeGenerator : public TemplateParametrizedEntity
{
public:
	explicit InitTerminateCodeGenerator(const QString &pathToTemplates);
	virtual ~InitTerminateCodeGenerator();

	/// Called each time before new generation process.
	/// Default implementation does nothing.
	virtual void reinit();

	/// Override must return code for initialization some robot part.
	/// Default implementation returns empty string.
	virtual QString initCode();

	/// Override must return code for deinitialization some robot part.
	/// Default implementation returns empty string.
	virtual QString terminateCode();

	/// Override must return code that will be invoked 1 time per 1 ms.
	/// Default implementation returns empty string.
	virtual QString isrHooksCode();
};

}
}
