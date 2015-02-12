#pragma once

#include <qrtext/core/parser/precedenceTable.h>
#include <qrtext/core/ast/node.h>

#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace lua {

/// Provides info about precedencies and associativity of the operators of
/// the target language (i.e. the language we generate to). Converts thus
/// lua ast nodes to target language`s operations.
class ROBOTS_GENERATOR_EXPORT PrecedenceConverterInterface
{
public:
	virtual ~PrecedenceConverterInterface() {}

	/// Returns a precedence of the operator represented by the given AST node.
	/// Bigger precedence numbers correspond to higher precedence.
	virtual int precedence(const qrtext::core::ast::Node &node) = 0;

	/// Returns the associativity of the binary operator represented by the given AST node.
	virtual qrtext::core::Associativity associativity(const qrtext::core::ast::Node &node) = 0;
};

}
}

