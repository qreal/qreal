#pragma once

#include <QtCore/QSet>

#include "initTerminateCodeGenerator.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace generatorBase {
namespace parts {

/// Inspects function blocks with initialization marker and generates code
/// for initialization section.
class Functions : public InitTerminateCodeGenerator
{
public:
	explicit Functions(const QString &pathToTemplates);

	void reinit() override;
	QString initCode() override;

	/// Adds given function block body into resulting initialization code.
	void registerFunctionInInitialization(const QString &body);

private:
	QStringList mFunctions;
};

}
}
