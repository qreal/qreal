#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>

#include <qrgui/mainwindow/projectManager/projectManagementInterface.h>
#include <qrgui/mainwindow/mainWindowInterpretersInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>

#include <robotsInterpreterCore/blocks/blockParserInterface.h>

namespace robotsInterpreterCore {
namespace interpreter {

class InterpreterInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~InterpreterInterface() {}

	virtual qReal::IdList providedBlocks() const = 0;

	// TODO: Unneeded.
	virtual blocks::BlockParserInterface &parser() const = 0;
};

}
}
