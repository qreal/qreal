#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include <QtGui/QIcon>

class EditorInterface
{
public:
    virtual ~EditorInterface() {}
    
    virtual void initPlugin() = 0;

    virtual QString getName(QString element) const = 0;
    virtual QIcon getIcon(QString element) const = 0;

    virtual QStringList diagrams() const = 0;
    virtual QStringList elements() const = 0;
};

Q_DECLARE_INTERFACE(EditorInterface, "ru.tepkom.QReal.EditorInterface/0.2")
