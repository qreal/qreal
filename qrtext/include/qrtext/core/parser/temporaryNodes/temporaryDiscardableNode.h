#pragma once

#include "qrtext/core/ast/node.h"

namespace qrtext {
namespace core {

/// Node that is used only for connection and shall be discarded from resulting tree.
class TemporaryDiscardableNode : public ast::Node {
};

}
}
