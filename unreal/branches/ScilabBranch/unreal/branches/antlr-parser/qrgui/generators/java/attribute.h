#include <QtCore/QTextStream>
#include <QtCore/QStringList>

class Attribute
{
public:
    Attribute(QString declaration);
    QString serializeMe();

    QString visibility;
    bool isStatic;
    bool isFinal;
    bool isVolatile;
    bool isTransient;
    QString type;
    QString name;
    QString defaultValue;
private:
    bool setBoolField(QString &declaration, QString fieldString);
};
