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
		QRealValue* clone() const;

	public:
		~QRealValue(){};

		QString toString() const;
		void fromString(QString const&);

		int toInteger() const;
		void fromInteger(int);

		bool toBoolean() const;
		void fromBoolean(bool);

		double toReal() const;
		void fromReal(double);

		friend class QRealType;
	};
};
