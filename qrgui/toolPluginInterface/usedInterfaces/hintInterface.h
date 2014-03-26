#pragma once

#include <qrkernel/ids.h>
#include <qrkernel/definitions.h>

namespace qReal {

class HintInterface {
public:
	Q_INVOKABLE void addHint(QString const &message);
	Q_INVOKABLE void arrowToWidget(QWidget const &target);
};

}
