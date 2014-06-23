#pragma once

#include <QtCore/QMetaType>

namespace interpreterBase {
namespace robotModel {

/// Direction of a port and device (input or output). Note that in some kits (like NXT) two devices can be
/// connected to one port simultaneously, like motor and encoder. Direction is used to differentiate them.
enum Direction
{
	/// Device provides data (like sensors or encoders).
	input = 0
	/// Device accepts data (like motors or lamps).
	, output
};

}
}

Q_DECLARE_METATYPE(interpreterBase::robotModel::Direction)
