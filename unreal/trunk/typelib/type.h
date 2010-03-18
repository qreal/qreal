#pragma once

#include <QString>
#include "value.h"

namespace qRealType {
	class qRealTypeFactory;

	// Basic OCL types
	enum QREAL_METATYPE {
		INTEGER,
		REAL,
		BOOLEAN,
		STRING,
	};

	class qRealType {
		QREAL_METATYPE type;
		QString name;
		qRealValue *defaultValue;

		qRealType(QREAL_METATYPE);
	public:
		~qRealType();

		QString toString();
		qRealValue& New();
	};
};
