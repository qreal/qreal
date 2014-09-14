#pragma once

#include <QtCore/QString>
#include <QtCore/QList>

namespace qReal {
namespace gestures {

class LevenshteinDistance
{
public:
	static int getLevenshteinDistance(QString const &key1, QString const &key2);
};

}
}
