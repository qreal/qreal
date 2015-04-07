#pragma once

#include <QtCore/QSharedPointer>

#include "ast/node.h"

namespace generationRules {

class TreeGeneratorFromString
{
public:
	static QSharedPointer<simpleParser::ast::Node> generatedTreeFromString(const QString &stream);
};

}
