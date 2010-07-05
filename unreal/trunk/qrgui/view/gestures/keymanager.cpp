#include "keymanager.h"
#include <QPointF>
#include <QString>

static const QString strBase64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

KeyManager::KeyManager()
{
}

QString KeyManager::getKey(QList<QPointF> const & path)
{
    analysePoints(path);
    QString key = "";
    QPointF pntLast (-1, -1);
    QPointF pntPath;
    if ((mLowerBound - mUpperBound) / iSize > unidimensionalLimit
        || (mRightBound - mLeftBound) / iSize > unidimensionalLimit)
    {
        foreach (QPointF point, path)
        {
            if((mLowerBound - mUpperBound) / iSize < unidimensionalLimit)
            {
                pntPath.setX((point.x() - mLeftBound) * iSize / (mRightBound - mLeftBound));
                pntPath.setY(0);
            }
            else if((mRightBound - mLeftBound) / iSize < unidimensionalLimit)
            {
                pntPath.setX(0);
                pntPath.setY((point.y() - mUpperBound) * iSize / (mLowerBound - mUpperBound));
            }
            else
            {
                pntPath.setX((point.x() - mLeftBound) * iSize / (mRightBound - mLeftBound));
                pntPath.setY((point.y() - mUpperBound) * iSize / (mLowerBound - mUpperBound));
            }
            if(pntPath.x() != pntLast.x() || pntPath.y() != pntLast.y())
            {
                pntLast = pntPath;
                key += strBase64[(int)(pntPath.x() + pntPath.y() * iSize)];
            }
        }
    }
    return key;
}

void KeyManager::analysePoints(QList<QPointF> const & path)
{
    if (path.size() > 0)
    {
        mLeftBound = path[0].x();
        mRightBound = path[0].x();
        mUpperBound = path[0].y();
        mLowerBound = path[0].y();
    }
    foreach (QPointF pnt, path)
    {
        if (pnt.x() < mLeftBound) mLeftBound = pnt.x();
        if (pnt.x() > mRightBound) mRightBound = pnt.x();
        if (pnt.y() > mLowerBound) mLowerBound = pnt.y();
        if (pnt.y() < mUpperBound) mUpperBound = pnt.y();
    }
}

void KeyManager::clear()
{
    mLeftBound = 0;
    mRightBound = 0;
    mUpperBound = 0;
    mLowerBound = 0;
}
