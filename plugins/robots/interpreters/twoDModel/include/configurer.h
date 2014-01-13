#pragma once

namespace twoDModel {

/// Can be used for customizing different 2D emulator aspects in concrete interpreter
class Configurer
{
public:
	virtual ~Configurer() {}

	// TODO: move physical constants here

	virtual QString robotImage() const = 0;

	// TODO: sensor images here

	// TODO: display emulator here
};

}
