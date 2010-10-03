#include "key8manager.h"
#include <cmath>
#include <QCharRef>

const QString str8 = "12345678";

Key8Manager::Key8Manager()
{
}

QString Key8Manager::getKey(QList<QPoint> const & path)
{
	QString key = "";
	int last = -1;
	for (int k = 1; k < path.size(); k++)
	{
		for (int i = 0; i < 8; i++)
		{
			double det1 = cos(pi / 8 - i * pi / 4) * (path[k].y() - path[k - 1].y())
						  - sin(pi / 8 - i * pi / 4) * (path[k].x() - path[k - 1].x());
			double det2 = cos(pi / 8 - (i + 1) * pi / 4) * (path[k].y() - path[k - 1].y())
						  - sin(pi / 8 - (i + 1) * pi / 4) * (path[k].x() - path[k - 1].x());
			if (det2 >= 0 && det1 < 0 && last != i)
			{
				last = i;
				key += str8[i];
			}
		}
	}
	return filterKey(key);
}

QString Key8Manager::filterKey(QString const &path)
{
	if (path.isEmpty())
		return "";

	QString result = "";

	result += path[0];

	for (int i = 1; i < path.length() - 1; ++i) {
		if (path[i - 1] == path[i + 1] && isClose(path[i - 1], path[i]))
			i += 2;
		else
			result += path[i];
	}

	if (path.length() > 1)
		result += path.right(1);

	return result;
}

bool Key8Manager::isClose(QChar a, QChar b)
{
	return a == b
			|| a == str8[(str8.indexOf(b) + str8.length() - 1) % str8.length()]
			||  a == str8[(str8.indexOf(b) + str8.length() + 1) % str8.length()]
			;
}
