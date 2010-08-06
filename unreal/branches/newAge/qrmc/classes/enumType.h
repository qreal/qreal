#pragma once

#include "nonGraphicType.h"

#include <QStringList>
#include <QDebug>

namespace qrmc {
	class EnumType : public NonGraphicType
	{
	public:
		EnumType(Diagram *diagram, qrRepo::RepoApi *api, qReal::Id const &id);

		virtual bool init(QString const &context);
		virtual Type* clone() const;

		void print();

		virtual QString generateEnums(QString const &lineTemplate) const;

	private:

		QStringList mValues;
	};
}
