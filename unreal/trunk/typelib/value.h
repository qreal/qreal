#pragma once

#include <QString>

namespace qRealType {
	class qRealType;

	class qRealValue {
		union qRealValueUnion {
			int integerVal;
			double realVal;
			QString *stringVal;
			bool booleanVal;
		};
		qRealType *type;
		qRealValueUnion value;
		qRealValue(qRealType *);
		qRealValue& clone();

	public:
		~qRealValue();

		QString toString();
		void fromString(QString const&);

		int toInteger();
		void fromInteger(int);

		bool toBoolean();
		void fromBoolean(bool);

		double toReal();
		void fromReal(double);

		friend class qRealType;
	};
};
