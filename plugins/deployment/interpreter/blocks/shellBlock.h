#pragma once

#include <qrutils/interpreter/block.h>

#include "shellWidget.h"

namespace deployment {
namespace blocks {

class ShellBlock : public qReal::interpretation::Block
{
public:
	explicit ShellBlock(ShellWidget *shellWidget);

	void run() override;

protected:
	/// Wraps the given string with the qoutes
	QString wrap(QString const &string) const;

private:
	/// May be overrided to run custom process. Default implementation runs bash.
	virtual QString processName() const;

	/// Overload must return arguments for starting process. Default implementation returns empty list.
	/// Non-constant couse arguments can be evaluated by lua interpreter.
	virtual QStringList arguments();

	ShellWidget *mShellWidget;  // Does not take ownership
};

}
}
