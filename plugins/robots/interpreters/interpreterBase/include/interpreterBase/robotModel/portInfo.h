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
	/// Constructs invalid PortInfo instance
	PortInfo();
	explicit PortInfo(QString const &name, QStringList const &nameAliases = QStringList());

	bool isValid() const;

	QString name() const;

	QStringList nameAliases() const;

private:
	QString mName;
	QStringList mNameAliases;
};

inline bool operator ==(PortInfo const &left, PortInfo const &right)
{
	return left.name() == right.name()
			&& left.nameAliases() == right.nameAliases();
}

inline bool operator !=(PortInfo const &left, PortInfo const &right)
{
	return !(left == right);
}

inline uint qHash(PortInfo const &key)
{
	return qHash(key.name());
}

}
}
