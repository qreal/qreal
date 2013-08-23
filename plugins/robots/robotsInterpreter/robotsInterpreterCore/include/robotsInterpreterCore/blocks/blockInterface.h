#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>

namespace robotsInterpreterCore {
namespace blocks {

class BlockInterface : public QObject
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

};

}
}
