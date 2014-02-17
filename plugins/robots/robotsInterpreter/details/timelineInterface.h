#pragma once

#include <QtCore/QtGlobal>

#include "abstractTimer.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

/// An interface for all objects that may measure time
class TimelineInterface
{
public:
	/// Returns time in ms passed from some point (no matter what point).
	virtual quint64 timestamp() const = 0;

	/// Creates new timer for specific implementation. Transfers ownership
	virtual AbstractTimer *produceTimer() = 0;
};

}
}
}
}
