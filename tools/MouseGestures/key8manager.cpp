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
    return key;
}
