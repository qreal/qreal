#pragma once

#include "qrtext/core/ast/node.h"

namespace qrtext {
namespace core {

/// Node that is used only for connection and shall be discarded from resulting tree. It represents non-interesting
/// parts of parse tree, like brackets, or optional parts of grammar that were not in input stream (in such case it is
/// not connected).
class TemporaryDiscardableNode : public ast::Node
{
};

}
}
