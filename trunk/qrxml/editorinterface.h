#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include <QtGui/QIcon>

class EditorInterface
{
public:
	virtual ~EditorInterface() {}

	virtual void initPlugin() = 0;
	virtual QString id() const = 0;

	virtual QStringList diagrams() const = 0;
	virtual QStringList elements(QString diagram) const = 0;

	virtual QIcon getIcon(QString element) const = 0;

	virtual QString editorName() const = 0;
	virtual QString diagramName(QString diagram) const = 0;
	virtual QString elementName(QString diagram, QString element) const = 0;
};

Q_DECLARE_INTERFACE(EditorInterface, "ru.tepkom.QReal.EditorInterface/0.3")
