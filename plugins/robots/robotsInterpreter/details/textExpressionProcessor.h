#pragma once

#include <QtCore/QMap>

#include <qrutils/expressionsParser/number.h>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class TextExpressionProcessor
{
public:
	QString processExpression(QString const &expression, QMap<QString, utils::Number> const &variables) const;
};

}
}
}
}
