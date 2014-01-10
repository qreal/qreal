#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT PortInfo
{
public:
	enum Direction {
		input
		, output
		, inOut
	};

	PortInfo(QString const &name, QStringList const &nameAliases, Direction direction);

	QString const &name() const;

	QStringList const &nameAliases() const;

	Direction direction() const;

private:
	QString const mName;
	QStringList const mNameAliases;
	Direction const mDirection;
};

}
}
