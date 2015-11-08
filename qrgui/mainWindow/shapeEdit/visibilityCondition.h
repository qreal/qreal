#pragma once

#include <QtCore/QString>

struct VisibilityCondition
{
    QString property;
    QString sign;
    QString value;

    bool operator==(const VisibilityCondition &other) const
    {
        return this->property == other.property && this->sign == other.sign
                && this->value == other.value;
    }

    bool operator!=(const VisibilityCondition &other) const
    {
        return !(*this == other);
    }
};
