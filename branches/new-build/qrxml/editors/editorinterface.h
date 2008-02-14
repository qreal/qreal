#include <QtCore/QObject>
#include <QtCore/QtPlugin>

class EditorInterface
{
public:
    virtual ~EditorInterface() {}

    virtual QStringList elements() const = 0;
    virtual void testLoad(QWidget *parent) = 0;
};

Q_DECLARE_INTERFACE(EditorInterface, "ru.tepkom.QReal.EditorInterface/0.1")
