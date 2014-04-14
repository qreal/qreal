#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>

#include "direction.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

/// A class describing a port on the robot where different devices can be plugged.
class ROBOTS_INTERPRETER_BASE_EXPORT PortInfo
{
public:
	/// Deserializes PortInfo instance from the string obtained by toString() method.
	static PortInfo fromString(QString const &string);

	/// Constructs invalid PortInfo instance.
	PortInfo();

	/// Constructs new PortInfo instance. If name is empty then the port is considered to be invalid.
	/// Ports with similar names but different directions are considered to be different.
	explicit PortInfo(QString const &name, Direction direction
			, QStringList const &nameAliases = QStringList()
			, QString const &reservedVariableName = QString());

	/// Returns true if this PortInfo is non-empty (i.e. really describes some port)
	bool isValid() const;

	/// Returns the name of the port.
	QString name() const;

	/// Returns the direction of this port. Ports with similar names but different
	/// directions are considered to be different.
	Direction direction() const;

	/// Returns a list of names that will be treated as corresponding to the same port as name() does.
	/// Useful for compability of diagrams for the different robot models (e.g. '1' for TRIK and 'A' for NXT).
	QStringList nameAliases() const;

	/// Returns a name of the reserved variable for this port (that varible can be used to access
	/// devices values on this port).
	QString reservedVariable() const;

	/// Serializes this PortInfo instance into the inner string representation.
	QString toString() const;

private:
	QString mName;
	Direction mDirection;
	QStringList mNameAliases;
	QString mReservedVariable;
};

inline bool operator ==(PortInfo const &left, PortInfo const &right)
{
	return left.name() == right.name()
			&& left.direction() == right.direction()
			&& left.nameAliases() == right.nameAliases();
}

inline bool operator !=(PortInfo const &left, PortInfo const &right)
{
	return !(left == right);
}

inline uint qHash(PortInfo const &key)
{
	return qHash(key.name()) ^ qHash(QString(key.direction() == input ? "input" : "output"));
}

inline bool operator <(PortInfo const &left, PortInfo const &right)
{
	if (left.name() == right.name()) {
		return left.direction() == input && right.direction() == output;
	}

	return left.name() < right.name();
}

}
}

Q_DECLARE_METATYPE(interpreterBase::robotModel::PortInfo)
