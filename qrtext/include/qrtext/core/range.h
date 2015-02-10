#pragma once

#include "qrtext/core/connection.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {

/// A range between two connections in code. Characterizes location of a token in code.
class QRTEXT_EXPORT Range
{
public:
	/// Constructor that takes start and end coordinates of a range.
	Range(const Connection &start, const Connection &end);

	/// Constructor of an invalid range object.
	Range();

	/// Returns start coordinate of a range.
	const Connection &start() const;

	/// Returns end coordinate of a range.
	const Connection &end() const;

private:
	Connection mStart;
	Connection mEnd;
};

}
}
