#pragma once

#include <QtCore/QString>
#include <QtCore/QTime>

#include <qrkernel/ids.h>

namespace qReal {
namespace gui {

class Hint {

public:
	Hint(QString const &message, qReal::Id  const &position);
	QString message() const;
	QString timestamp() const;
	qReal::Id  position() const;

private:
	QString mMessage;
	QTime mTimestamp;
	qReal::Id  mPosition;
};
}
}
