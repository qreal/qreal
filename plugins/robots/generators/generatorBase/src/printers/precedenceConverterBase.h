#include "precedenceConverterInterface.h"

#include <qrtext/lua/luaAstVisitorInterface.h>

namespace generatorBase {
namespace printing {

/// Provides converting precedence table base implementation with typical operators precedence.
class PrecedenceConverterBase : public PrecedenceConverterInterface
		, public qrtext::lua::LuaAstVisitorInterface
{
public:
	int precedence(qrtext::core::ast::Node const &node) override;
	qrtext::core::Associativity associativity(qrtext::core::ast::Node const &node) override;

private:
	void returnPrecedence(int precedence);
	void returnAssociativity(qrtext::core::Associativity associativity);

	void visit(qrtext::lua::ast::UnaryMinus const &node) override;
	void visit(qrtext::lua::ast::Not const &node) override;
	void visit(qrtext::lua::ast::BitwiseNegation const &node) override;
	void visit(qrtext::lua::ast::LogicalAnd const &node) override;
	void visit(qrtext::lua::ast::LogicalOr const &node) override;
	void visit(qrtext::lua::ast::Addition const &node) override;
	void visit(qrtext::lua::ast::Subtraction const &node) override;
	void visit(qrtext::lua::ast::Multiplication const &node) override;
	void visit(qrtext::lua::ast::Division const &node) override;
	void visit(qrtext::lua::ast::IntegerDivision const &node) override;
	void visit(qrtext::lua::ast::Modulo const &node) override;
	void visit(qrtext::lua::ast::BitwiseAnd const &node) override;
	void visit(qrtext::lua::ast::BitwiseOr const &node) override;
	void visit(qrtext::lua::ast::BitwiseXor const &node) override;
	void visit(qrtext::lua::ast::BitwiseLeftShift const &node) override;
	void visit(qrtext::lua::ast::BitwiseRightShift const &node) override;
	void visit(qrtext::lua::ast::Concatenation const &node) override;
	void visit(qrtext::lua::ast::Equality const &node) override;
	void visit(qrtext::lua::ast::LessThan const &node) override;
	void visit(qrtext::lua::ast::LessOrEqual const &node) override;
	void visit(qrtext::lua::ast::Inequality const &node) override;
	void visit(qrtext::lua::ast::GreaterThan const &node) override;
	void visit(qrtext::lua::ast::GreaterOrEqual const &node) override;

	int mLastRequestPresedence;
	qrtext::core::Associativity mLastRequestAssociativity;
};

}
}
