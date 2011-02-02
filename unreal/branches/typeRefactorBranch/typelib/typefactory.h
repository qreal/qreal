#pragma once

#include <QString>
#include <QtCore/QMap>
#include <QtCore/QList>
#include "type.h"
#include "constraint.h"

namespace qRealType {
	class QRealTypeFactory {
		QMap<QString, QRealType*> mTypes;

		QRealTypeFactory();
	public:
		~QRealTypeFactory();

		static void init();
		static QRealType* getTypeByName(QString const &name);

		QRealType* getTypeByName_(QString const &name) const;

		static QRealType* newSubType(QString const &name, QConstraintList const &constr, QRealValue const *def, QRealType *type);
		QRealType* newSubType_(QString const &name, QConstraintList const &constr, QRealValue const *def, QRealType *type);
	};
};
