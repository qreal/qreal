#pragma once

#include <QtCore/QSet>

#include "initTerminateCodeGenerator.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace qReal {
namespace robots {
namespace generators {
namespace parts {

/// Inspects function blocks with initialization marker and generates code
/// for initialization section.
class Functions : public InitTerminateCodeGenerator
{
public:
	explicit Functions(QString const &pathToTemplates);

	void reinit() override;
	QString initCode() override;

	/// Adds given function block body into resulting initialization code.
	void registerFunctionInInitialization(QString const &body);

private:
	QStringList mFunctions;
};

}
}
}
}
