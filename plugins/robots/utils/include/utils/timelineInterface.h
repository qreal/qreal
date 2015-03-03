#pragma once

#include <QtCore/QtGlobal>

namespace utils {

class AbstractTimer;

/// An interface for all objects that may measure time
class TimelineInterface
{
public:
	virtual ~TimelineInterface() {}

	/// Returns time in ms passed from some point (no matter what point).
	virtual quint64 timestamp() const = 0;

	/// Creates new timer for specific implementation. Transfers ownership.
	virtual AbstractTimer *produceTimer() = 0;
};

}
