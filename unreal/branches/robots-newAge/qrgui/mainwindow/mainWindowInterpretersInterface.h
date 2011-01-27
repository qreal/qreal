#pragma once

#include "../kernel/ids.h"

namespace qReal {
namespace gui {

class ErrorReporter;

class MainWindowInterpretersInterface
{
public:
	~MainWindowInterpretersInterface() {};
	virtual void selectItem(Id const &graphicalId) = 0;
	virtual void highlight(Id const &graphicalId, bool exclusive = true) = 0;
	virtual void dehighlight(Id const &graphicalId) = 0;
	virtual ErrorReporter *errorReporter() = 0;
};

}
}
