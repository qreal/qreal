#include "xmlparser.h"
#include "adopter.h"
#include <QFile>
#include <QDomElement>
#include <QStringList>
#include <QTextStream>
#include <QDebug>

const QString gestureKeyWord = "gesture";
const QString idealPathKeyWord = "idealPath";
const QString nameKeyWord = "name";
const QString userPathKeyWord = "userPath";
const QString pathKeyWord = "path";

QMap<QString, UsersGestures>
        XmlParser::parseXml(QString const & pathToFile)
{
    QMap<QString, UsersGestures> gestures;
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
            PathVector points = Parser::stringToPath(path);
            QPair<PathVector, QStringList> usersGestures(points,
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

void XmlParser::save(const QMap<QString, UsersGestures> &map,
                     const QString &pathToFile)
{
    QFile file(pathToFile);
    if (!file.open(QFile::ReadWrite))
        return;
    QDomDocument doc;
    QDomElement root = doc.createElement("usersGestures");
    doc.appendChild(root);
    saveDocument(map, doc, pathToFile);
}

void XmlParser::saveMore(const QMap<QString, UsersGestures> &mapGestures, const QString &fileName)
{
    QFile file(fileName);
    QMap<QString, UsersGestures> map = mapGestures;
    if (!file.exists())
    {
        save(map, fileName);
        return;
    }
    if (!file.open(QIODevice::ReadWrite))
        return;
    QDomDocument doc("document");
    doc.setContent(&file);
    file.close();
    QDomNodeList elements = doc.elementsByTagName(gestureKeyWord);
    for (int i = 0; i < elements.size(); i++)
    {
        QDomElement element = elements.at(i).toElement();
        QString name = element.attribute(nameKeyWord);
        UsersGestures usersGestures = map[name];
        foreach (QString gesture, usersGestures.second)
        {
            QDomElement usersPath = doc.createElement(userPathKeyWord);
            usersPath.setAttribute(pathKeyWord, gesture);
            element.appendChild(usersPath);
        }
        map.remove(name);
    }
    saveDocument(map, doc, fileName);
}

void XmlParser::saveDocument(const QMap<QString, UsersGestures> &map, QDomDocument doc,
                             QString const & fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite))
        return;
    QTextStream textStream(&file);
    foreach (QString name, map.keys())
    {
        QDomElement element = doc.createElement(gestureKeyWord);
        element.setAttribute(nameKeyWord, name);
        element.setAttribute(idealPathKeyWord, Parser::pathToString(map[name].first));
        foreach (QString usersGesture, map[name].second)
        {
            QDomElement usersPath = doc.createElement(userPathKeyWord);
            usersPath.setAttribute(pathKeyWord, usersGesture);
            element.appendChild(usersPath);
        }
        doc.documentElement().appendChild(element);
    }
    qDebug() << doc.elementsByTagName("usersGestures").size();
    doc.save(textStream, 2);
    file.close();
}
