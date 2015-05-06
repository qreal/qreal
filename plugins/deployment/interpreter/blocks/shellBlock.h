/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QProcess>

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

	QProcess *mProcess;
	QString mLastOutput;
	QString mLastError;

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
