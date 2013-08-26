#pragma once

#include "abstractSimpleGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class BindingGenerator : public AbstractSimpleGenerator
{
public:
	class ConverterInterface
	{
	public:
		virtual ~ConverterInterface();

		virtual QString convert(QString const &data) = 0;
	};

	class EmptyConverter : public ConverterInterface
	{
	public:
		virtual QString convert(QString const &data);
	}

	BindingGenerator(LogicalModelAssistInterface const &model
			, Id const &id
			, QList<>
			, QObject *parent = 0);
};

}
}
}
}
