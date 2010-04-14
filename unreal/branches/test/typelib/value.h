#pragma once

#include <QString>

namespace qRealType {
	class QRealType;

	class QRealValue {
		union QRealValueUnion {
			int integerVal;
			double realVal;
			QString *stringVal;
			bool booleanVal;
		};
		QRealType *mType;
		QRealValueUnion mValue;
		QRealValue(QRealType *type);

	public:
		~QRealValue(){};

		QRealValue* clone() const;

		// Handy proxies
		QString toString() const;
		void fromString(QString const& val);
		int toInteger() const;
		void fromInteger(int val);
		bool toBoolean() const;
		void fromBoolean(bool val);
		double toReal() const;
		void fromReal(double val);

		friend class QRealType;
	};
};
