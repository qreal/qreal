#include <QtCore/QObject>
#include <QtCore/QtPlugin>

class EditorInterface
{
public:
    virtual ~EditorInterface() {}
    
    virtual void initPlugin() = 0;
    virtual QStringList diagrams() const = 0;
    virtual QStringList elements(int idx) const = 0;
};

Q_DECLARE_INTERFACE(EditorInterface, "ru.tepkom.QReal.EditorInterface/0.2")
