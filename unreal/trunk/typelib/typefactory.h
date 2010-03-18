#pragma once

#include <QString>
#include <QMap>
#include "type.h"

namespace qRealType {
	class QRealTypeFactory {
		QMap<QString, QRealType*> mTypes;

		QRealTypeFactory();
	public:
		~QRealTypeFactory();

		static void init();
		static QRealType* getTypeByName(QString name);

		QRealType* getTypeByName_(QString name) const;
	};
};
