#pragma once

#include "nonGraphicType.h"

#include <QStringList>
#include <QDebug>

namespace qrmc {
	class EnumType : public NonGraphicType
	{
	public:
		EnumType(Diagram *diagram, qrRepo::LogicalRepoApi *api, qReal::Id const &id);

		virtual bool init(const QString &context);
		virtual Type* clone() const;

		void print();

		virtual QString generateEnums(const QString &lineTemplate) const;

	private:

		QStringList mValues;
	};
}
