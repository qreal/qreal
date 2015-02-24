#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class Identifier : public Node
{
public:
	Identifier(QString const &name)
		: mName(name)
	{
	}

	QString const &name() const {
		return mName;
	}

private:
//	void accept(core::AstVisitorInterface &visitor) const override
//	{
//		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(*this);
//	}

	QString mName;
};

}
}
