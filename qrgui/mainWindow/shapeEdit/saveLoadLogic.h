#pragma once

#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtCore/QPersistentModelIndex>
#include <QtXml/QDomDocument>

#include <qrutils/outFile.h>

#include "mainWindow/shapeEdit/iShapeEdit.h"
#include "mainWindow/shapeEdit/scene.h"
#include "mainWindow/shapeEdit/xmlLoader.h"
#include "mainWindow/shapeEdit/propertyInfo.h"

namespace qReal {
namespace shapeEdit {

class SaveLoadLogic : public QObject
{
    Q_OBJECT

public:
    SaveLoadLogic(IShapeEdit *parent, Scene *scene, const bool isUsingTypedPorts = false);
    virtual ~SaveLoadLogic();

public slots:
    void loadFromFile(const QString &fileName);
    void loadFromText(const QString &text);

    QStringList loadPortTypes();
    QMap<QString, PropertyInfo> loadProperties();

    void save();
    void saveToXml(const QString &fileName);
    void saveAsPicture(const QString &fileName);

protected:
    virtual void doSave();
    virtual void doLoadPortTypes(QStringList &result);
    virtual void doLoadProperties(QMap<QString, PropertyInfo> &result);

    QDomDocument generateDom();
    QList<QDomElement> generateGraphics(QDomDocument &doc);

    IShapeEdit *mParent; // No ownersip
    Scene *mScene; // No ownersip
    const bool mIsUsingTypedPorts;
    XmlLoader mParser;

    QString const mXmlTitle = "<?xml version='1.0' encoding='utf-8'?>\n";
};

}
}
