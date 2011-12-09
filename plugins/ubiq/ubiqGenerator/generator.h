#pragma once

#include <QtCore/QObject>

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace ubiq {
namespace generator {

/// Main class of UbiqMobile generator, directs generation process.
class Generator : public QObject
{
	Q_OBJECT

public:
	Generator();
	virtual ~Generator();

	/// Initializes generator with logical model reference and means to report errors.
	void init(qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

public slots:
	/// Starts generation process.
	void generate();

private:
	/// Logical model, from which generator takes all information.
	qReal::LogicalModelAssistInterface const *mLogicalModel;

	/// Error reporter object, used to show generation errors in output window.
	qReal::ErrorReporterInterface *mErrorReporter;
};

}
}
