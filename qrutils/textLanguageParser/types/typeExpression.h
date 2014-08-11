#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {
namespace types {

/// Base class for all type expressions.
class QRUTILS_EXPORT TypeExpression {
public:
	template<typename Type>
	bool is()
	{
		return dynamic_cast<Type *>(this) != nullptr;
	}
};

}
}
