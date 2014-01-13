#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>

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

inline bool operator ==(PortInfo const &left, PortInfo const &right)
{
	return left.direction() == right.direction()
			&& left.name() == right.name()
			&& left.nameAliases() == right.nameAliases();
}

inline bool operator !=(PortInfo const &left, PortInfo const &right)
{
	return !(left == right);
}

inline uint qHash(PortInfo const &key)
{
	return key.direction() ^ qHash(key.name());
}

}
}
