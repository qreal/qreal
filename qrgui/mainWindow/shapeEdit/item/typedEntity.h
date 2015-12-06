#pragma once

#include <QtCore/QString>

class TypedEntity
{
public:
    TypedEntity();

    void setType(const QString &type);

    QString getType() const;

    virtual ~TypedEntity();

protected:
    QString mType;
};
