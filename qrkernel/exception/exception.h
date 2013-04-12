#pragma once

#include <QtCore/QString>

#include "../kernelDeclSpec.h"

namespace qReal {

/// Base class for all QReal exceptions. Thrown when "something is wrong"
/// in QReal code.
class QRKERNEL_EXPORT Exception
{
public:
	/// Constructor.
	/// @param message Error message.
	explicit Exception(QString const &message);

	/// Get error message.
	QString message() const;

private:
	/// Error message.
	QString const mMessage;
};

}
