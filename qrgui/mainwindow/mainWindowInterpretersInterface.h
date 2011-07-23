#pragma once

#include "../../qrkernel/ids.h"
#include "../toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace qReal {
namespace gui {

class ErrorReporter;

class MainWindowInterpretersInterface
{
public:
	virtual ~MainWindowInterpretersInterface() {}
	virtual void selectItem(Id const &graphicalId) = 0;
	virtual void highlight(Id const &graphicalId, bool exclusive = true) = 0;
	virtual void dehighlight(Id const &graphicalId) = 0;
	virtual void dehighlight() = 0;
	virtual ErrorReporterInterface *errorReporter() = 0;
};

}
}
