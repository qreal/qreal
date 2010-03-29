#pragma once
#include "IKeyManager.h"
#include <QPoint>
#include <QList>

class MouseMovementManager : public IKeyManager
{
public:
    MouseMovementManager();
    QString getKey(QList<QPoint> const & path);

private:
    void analysePoints(QList<QPoint> const & path);
    void clear();
    int mLeftBound;
    int mRightBound;
    int mUpperBound;
    int mLowerBound;
    static const int inMovement = 20;
    static const int iSize = 6;
    static const int unidimensionalLimit = 4;
};
