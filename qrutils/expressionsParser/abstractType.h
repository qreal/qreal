#pragma once
#include <QVariant>

#include "../utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT AbstractType
{
public:
	enum Type{
		number,
		array
	};

public:
	virtual ~AbstractType();
	virtual QVariant evaluate() = 0;
	virtual QString toString() const = 0;
	Type type() const;
	void setType(Type t);
	QString typeToString();
protected:
	Type mType;
};
}
