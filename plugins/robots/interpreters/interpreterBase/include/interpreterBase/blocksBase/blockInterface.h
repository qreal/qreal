#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>
#include <interpreterBase/interpreterBaseDeclSpec.h>

namespace interpreterBase {
namespace blocksBase {

class ROBOTS_INTERPRETER_BASE_EXPORT BlockInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~BlockInterface() {}

	/// Starts block execution.
	virtual void interpret() = 0;

	virtual void setFailedStatus() = 0;
	virtual void setIdleStatus() = 0;

	virtual qReal::Id const id() const = 0;

	/// Called each time when control flow has reached the end block of the requested for stepping into diagram.
	virtual void finishedSteppingInto() = 0;

signals:
	void done(qReal::Id const &nextBlock);
	void newThread(qReal::Id const &startBlock);
	void failure();

	/// Emitted each time when execution must be continued from the initial block
	/// of the specified diagram
	void stepInto(qReal::Id const &diagram);
};

}
}
