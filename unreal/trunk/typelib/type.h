#pragma once

#include <QString>
#include "value.h"

namespace qRealType {
	class QRealTypeFactory;

	// Basic OCL types
	enum QREAL_METATYPE {
		INTEGER,
		REAL,
		BOOLEAN,
		STRING,
	};

	class QRealType {
		QREAL_METATYPE mType;
		QString mName;
		QRealValue *mDefaultValue;

		QRealType(QREAL_METATYPE type);
	public:
		~QRealType();

		QString toString() const;
		QRealValue* newValue();

		friend class QRealTypeFactory;
	};
};
