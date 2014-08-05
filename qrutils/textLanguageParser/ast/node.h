#pragma once

#include "connection.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace ast {

/// Base class for all abstract syntax tree nodes.
class QRUTILS_EXPORT Node {
public:
	virtual ~Node() {}
};

}
}
