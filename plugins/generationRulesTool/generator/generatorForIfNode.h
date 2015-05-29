#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "ast/ifNode.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"

namespace generationRules {
namespace generator {

class GeneratorForIfNode
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::IfNode> ifNode
			, GeneratorConfigurer generatorConfigurer);
};

}
}
