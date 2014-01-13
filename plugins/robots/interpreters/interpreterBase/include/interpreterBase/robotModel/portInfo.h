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
		input = 0
		, output
		, inOut
	};

	PortInfo(QString const &name, Direction direction, QStringList const &nameAliases = QStringList());

	QString name() const;

	Direction direction() const;

	QStringList nameAliases() const;

private:
	QString mName;
	Direction mDirection;
	QStringList mNameAliases;
};

}
}
