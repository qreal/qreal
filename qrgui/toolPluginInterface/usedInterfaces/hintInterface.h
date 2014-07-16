#pragma once

#include <qrkernel/ids.h>
#include <qrkernel/definitions.h>

namespace qReal {

class HintInterface
{
public:
	///Send message as HintReporter.
	Q_INVOKABLE void addHint(QWidget *parent, QString const &message, int const duration);

	///Draw arrow near target.
	Q_INVOKABLE void arrowToWidget(QWidget *target, qreal angle);
};

}
