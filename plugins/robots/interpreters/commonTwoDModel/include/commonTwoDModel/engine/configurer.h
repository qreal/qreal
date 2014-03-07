#pragma once

#include <QtCore/QString>

namespace twoDModel {

/// Can be used for customizing different 2D emulator aspects in concrete interpreter
class Configurer
{
public:
	virtual ~Configurer() {}

	/// @todo move physical constants here

	virtual QString robotImage() const = 0;

	/// @todo sensor images here

	/// @todo display emulator here
};

}
