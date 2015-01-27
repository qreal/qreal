#pragma once

#include "qrtext/core/ast/node.h"

namespace qrtext {
namespace core {

/// Node that is used for error propagation and shall not appear in correct tree.
class TemporaryErrorNode : public ast::Node
{
};

}
}
