#pragma once

#include <generatorBase/simpleGenerators/binding.h>

namespace ev3 {
namespace converters {

class GoToBlockNumberConverter : public generatorBase::simple::Binding::ConverterInterface
{
public:
	GoToBlockNumberConverter(QString const &goToBlockNumber);

	QString convert(QString const &data) const override;

private:
	QString mGoToBlockNumber;
};

}
}

