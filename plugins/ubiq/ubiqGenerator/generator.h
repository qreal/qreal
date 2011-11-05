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

	void init(qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

public slots:
	/// Starts generation process.
	void generate();

private:
	qReal::LogicalModelAssistInterface const *mLogicalModel;

	qReal::ErrorReporterInterface *mErrorReporter;
};

}
}
