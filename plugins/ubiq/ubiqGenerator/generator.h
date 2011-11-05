#pragma once

#include <QtCore/QObject>

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"

namespace ubiq {
namespace generator {

/// Main class of UbiqMobile generator, directs generation process.
class Generator : public QObject
{
	Q_OBJECT

public:
	Generator();
	virtual ~Generator();

	void init(qReal::LogicalModelAssistInterface const &logicalModel);

public slots:
	/// Starts generation process.
	void generate();

private:
	qReal::LogicalModelAssistInterface const *mLogicalModel;
};

}
}
