#include "xmlparser.h"
#include "adopter.h"
#include <QFile>
#include <QDomElement>
#include <QStringList>
#include <QTextStream>

const QString gestureKeyWord = "gesture";
const QString idealPathKeyWord = "idealPath";
const QString nameKeyWord = "name";
const QString userPathKeyWord = "userPath";
const QString pathKeyWord = "path";

QMap<QString, QPair<QList<QPoint>, QList<QString> > >
                XmlParser::parseXml(QString const & pathToFile)
{
        QMap<QString, QPair<QList<QPoint>, QList<QString> > > gestures;
        QFile file(pathToFile);

        if (!file.exists())
                file.setFileName("../" + pathToFile);

        QDomDocument doc("document");
        if (!file.open(QIODevice::ReadWrite))
                return gestures;
        doc.setContent(&file);
        file.close();
        QDomNodeList elements = doc.elementsByTagName(gestureKeyWord);
        for (int i = 0; i < elements.size(); i++)
        {
                QDomElement element = elements.at(i).toElement();
                QString path = element.attribute(idealPathKeyWord);
                QString name = element.attribute(nameKeyWord);
                QDomNodeList paths = element.elementsByTagName(userPathKeyWord);
                if (!path.isEmpty())
                {
                        QList<QPoint> points = Parser::stringToPath(path);
                        QPair<QList<QPoint>, QList<QString> > usersGestures(points,
                                                                                                                getUsersGestures(paths));
                        gestures.insert(name, usersGestures);
                }
        }
        return gestures;
}

QList<QString> XmlParser::getUsersGestures(const QDomNodeList &list)
{
        QList<QString> gestures;
        for (int i = 0; i < list.size(); i++)
        {
                gestures.push_back(list.at(i).toElement().attribute(pathKeyWord, ""));
        }
        return gestures;
}
