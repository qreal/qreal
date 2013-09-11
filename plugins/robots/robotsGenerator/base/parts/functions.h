#pragma once

#include <QtCore/QSet>

#include "initTerminateCodeGenerator.h"
#include "../simpleGenerators/binding.h"

namespace qReal {
namespace robots {
namespace generators {
namespace parts {

class Functions : public InitTerminateCodeGenerator
{
public:
	Functions(QString const &pathToTemplates);

	virtual void reinit();
	virtual QString initCode();

	void registerFunctionInInitialization(QString const &body);

private:
	QStringList mFunctions;
};

}
}
}
}
