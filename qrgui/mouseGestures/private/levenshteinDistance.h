#pragma once

#include <QtCore/QString>
#include <QtCore/QList>

namespace qReal {
namespace gestures {

class LevenshteinDistance
{
public:
	static int getLevenshteinDistance(const QString &key1, const QString &key2);
};

}
}
