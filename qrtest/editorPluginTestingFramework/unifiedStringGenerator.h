#pragma once

#include <QtCore/QString>

namespace editorPluginTestingFramework {

class UnifiedStringGenerator
{
public:
	/// clones StringGenerator and inits cloned generator
	virtual UnifiedStringGenerator* clone() const = 0;

	/// generates string of results for one method of one interface
	virtual QString generateString() const = 0;

	/// generates name of tested method
	virtual QString methodName() const = 0;
};

}
