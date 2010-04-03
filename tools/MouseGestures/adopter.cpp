#include "adopter.h"
#include "math.h"

const QChar openBracket = '(';
const QChar closeBracket = ')';
const QChar comma = ',';
const QChar delimeter = ' ';
const double mouseSpeed = 5;

enum State
{
    OpenBracket,
    FirstNumber,
    FirstNumberEnd,
    Comma,
    SecondNumber,
    CloseBracket
};

Adopter::Adopter()
{
}

QList<QPoint> Adopter::getMousePath(QList<QPoint> const & path)
{
    QList<QPoint> mousePath;
    if (path.isEmpty())
    {
        return mousePath;
    }
    mousePath.append(path[0]);
    for (int i = 1; i < path.size(); i++)
    {
        QPoint currentPoint = path[i];
        QPoint previousPoint = path[i - 1];
        int length = currentPoint.x() - previousPoint.x();
        int width = currentPoint.y() - previousPoint.y();
        int number = (int) ((sqrt(pow(length, 2) + pow(width, 2))) / mouseSpeed + 1);
        for (int j = 1; j <= number; j++)
        {
            QPoint point(int(previousPoint.x() + length * j / number),
                         int(previousPoint.y() + width * j / number));
            mousePath.append(point);
        }
    }
    return mousePath;
}

bool Adopter::isLine(QList<QPoint> const & path)
{
    const int minDifference = 8;
    const double minCos = 0.8;
    int difference = path.size();
    while(difference >= minDifference)
    {
        for (int i = 0; i < path.size() / difference; i++)
        {
            double cos = getCos(path[i * difference], path[((2 * i + 1) * difference - 1) / 2],
                                path[(i + 1) * difference - 1]);
            if (cos < minCos)
            {
                return false;
            }
        }
        difference /= 2;
    }
    return true;
}

double Adopter::getCos(QPoint const & pnt1, QPoint const & pnt2, QPoint const & pnt3)
{
    QPoint vector1(pnt2.x() - pnt1.x(), pnt2.y() - pnt1.y());
    QPoint vector2(pnt3.x() - pnt2.x(), pnt3.y() - pnt2.y());
    double length1 = getLength(vector1);
    double length2 = getLength(vector2);
    if(length1 > 0 && length2 > 0)
    {
        return (double) getScalarProduct(vector1, vector2) / (length1 * length2);
    }
    return 1;
}

double Adopter::getScalarProduct(QPoint const & vector1, QPoint const & vector2)
{
    return vector1.x() * vector2.x() + vector1.y() * vector2.y();
}

double Adopter::getLength(QPoint const & vector)
{
    return sqrt(pow(vector.x(), 2) + pow(vector.y(), 2));
}

QString Adopter::pathToString(QList<QPoint> const & path)
{
    QString str = "";
    foreach (QPoint point, path)
    {
        str += openBracket + QString("%0").arg(point.x()) + comma
               + QString("%1").arg(point.y()) + closeBracket + delimeter;
    }
    return str;
}

bool Adopter::isPath(QString const & str)
{
    State currentState = CloseBracket;
    foreach (QChar symbol, str)
    {
        switch (currentState)
        {
        case OpenBracket:
            if (isDigit(symbol))
                currentState = FirstNumber;
            else if (symbol != delimeter)
                return false;
            break;
        case FirstNumber:
            if (!isDigit(symbol))
            {
                if (symbol == comma)
                    currentState = Comma;
                else if (symbol == delimeter)
                    currentState = FirstNumberEnd;
                else
                    return false;
            }
            break;
        case FirstNumberEnd:
            if (symbol == comma)
                currentState = Comma;
            else if (symbol != delimeter)
                return false;
            break;
        case Comma:
            if (isDigit(symbol))
                currentState = SecondNumber;
            else if (symbol != delimeter)
                return false;
            break;
        case SecondNumber:
            if (!isDigit(symbol))
            {
                if (symbol == closeBracket)
                    currentState = CloseBracket;
                else if (symbol != delimeter)
                    return false;
            }
            break;
        case CloseBracket:
            if (symbol == openBracket)
                currentState = OpenBracket;
            else if (symbol != delimeter)
                return false;
            break;
        }
    }
    return currentState == CloseBracket;
}
bool Adopter::isDigit(QChar const & symbol)
{
    return (symbol >= '0' && symbol <='9');
}

QList<QPoint> Adopter::stringToPath(QString const & str)
{
    QList<QPoint> path;
    if (!isPath(str))
    {
        return path;
    }
    QPoint currentPoint(0, 0);
    bool isInt;
    QString integerStr = "";
    bool isSecondPos = false;
    foreach (QChar symbol, str)
    {
        if (isDigit(symbol))
        {
            integerStr += symbol;
        }
        else
        {
            if (integerStr != "")
            {
                if (isSecondPos)
                {
                    currentPoint.setY(integerStr.toInt(&isInt, 10));
                    path.push_back(currentPoint);
                }
                else
                {
                    currentPoint.setX(integerStr.toInt(&isInt, 10));
                }
                isSecondPos = !isSecondPos;
            }
            integerStr = "";
        }
    }
    return path;
}
