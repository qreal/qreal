#pragma once

#include "qrtext/core/connection.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {

/// A range between two connections in code. Characterizes location of a token in code.
class QRTEXT_EXPORT Range {
public:
	/// Constructor that takes start and end coordinates of a range.
	Range(Connection const &start, Connection const &end);

	/// Constructor of an invalid range object.
	Range();

	/// Returns start coordinate of a range.
	Connection const &start() const;

	/// Returns end coordinate of a range.
	Connection const &end() const;

private:
	Connection mStart;
	Connection mEnd;
};

}
}
