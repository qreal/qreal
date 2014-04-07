#pragma once

#include <QtCore/QString>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

/// Can be used for customizing different 2D emulator aspects in concrete interpreter
class COMMON_TWO_D_MODEL_EXPORT Configurer
{
public:
	virtual ~Configurer() {}

	/// @todo move physical constants here

	virtual QString robotImage() const = 0;

	/// @todo sensor images here

	/// @todo display emulator here
};

}
