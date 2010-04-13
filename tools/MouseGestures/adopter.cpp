#include "adopter.h"
#include "math.h"

const QChar openBracket = '(';
const QChar closeBracket = ')';
const QChar comma = ',';
const QChar delimeter = ' ';
const QChar minus = '-';

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
            if (isDigit(symbol) || symbol == minus)
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
            if (isDigit(symbol) || symbol == minus)
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
        if (isDigit(symbol) || symbol == minus)
        {
            integerStr += symbol;
        }
        else
        {
            if (integerStr != "")
            {
                if (isSecondPos)
                {
                    currentPoint.setY(integerStr.toInt(&isInt, 0));
                    path.push_back(currentPoint);
                }
                else
                {
                    currentPoint.setX(integerStr.toInt(&isInt, 0));
                }
                isSecondPos = !isSecondPos;
            }
            integerStr = "";
        }
    }
    return path;
}
