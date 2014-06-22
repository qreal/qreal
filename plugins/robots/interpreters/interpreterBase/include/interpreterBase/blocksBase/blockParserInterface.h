#pragma once

#include <qrkernel/ids.h>
#include <qrutils/expressionsParser/number.h>

namespace interpreterBase {
namespace blocksBase {

/// An interface of any text language evaluator.
/// @todo Some method names and overall "look and feel" of this class are ugly.
class BlockParserInterface
{
public:
	virtual ~BlockParserInterface() {}

	/// Executes the expression that returns some numeric value. Returns the evaluated value.
	/// Transfers ownership.
	virtual utils::Number *standartBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId) = 0;

	/// Executes the expression that just performs some variables assignment and returns nothing.
	virtual void functionBlockParseProcess(QString const &stream, int &pos, qReal::Id const &curId) = 0;

	/// Executes the expression that returns some boolean value. Returns the evaluated value.
	virtual bool parseCondition(QString const &stream, int& pos, qReal::Id const &curId) = 0;

	/// Resets the error indicator.
	virtual void deselect() = 0;

	/// Resets true if some errors occured from the last deselect() method call.
	virtual bool hasErrors() const = 0;

	/// Returns a list of all variables and their values met during the parsing process.
	/// Does not transfer ownership.
	virtual QMap<QString, utils::Number *> const &variables() const = 0;
};

}
}
