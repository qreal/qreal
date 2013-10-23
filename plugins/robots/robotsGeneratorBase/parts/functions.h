#pragma once

#include <QtCore/QSet>

#include "initTerminateCodeGenerator.h"
#include "simpleGenerators/binding.h"

namespace qReal {
namespace robots {
namespace generators {
namespace parts {

/// Inspects function blocks with initialization marker and generates code
/// for initialization section.
class Functions : public InitTerminateCodeGenerator
{
public:
	Functions(QString const &pathToTemplates);

	virtual void reinit();
	virtual QString initCode();

	/// Adds given function block body into resulting initialization code.
	void registerFunctionInInitialization(QString const &body);

private:
	QStringList mFunctions;
};

}
}
}
}
