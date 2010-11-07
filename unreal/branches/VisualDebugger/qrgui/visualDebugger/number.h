#pragma once

#include <QVariant>

namespace qReal {
	class number {
		public:
			enum Type {
				Double = 0,
				Int
			};
	
		public:
			number(QVariant n, Type t);
			number();
			~number();
			
			QVariant property(QString name);
			void setProperty(QString name, QVariant value);
			
			void operator+=(number add);
			void operator-=(number sub);
			void operator*=(number mult);
			void operator/=(number div);
			number operator-();
			bool operator<(number arg);
			bool operator>(number arg);
			bool operator==(number arg);
			bool operator<=(number arg);
			bool operator>=(number arg);
			bool operator!=(number arg);
		private:
			QVariant mNumber;
			Type mType;
	};
}
