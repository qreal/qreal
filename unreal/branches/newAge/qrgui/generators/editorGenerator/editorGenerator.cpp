#include "editorGenerator.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QMessageBox>

#include "../../kernel/roles.h"
#include "../../../qrrepo/repoApi.h"

#include "../../../utils/outFile.h"

using namespace qReal;
using namespace generators;
using namespace utils;

EditorGenerator::EditorGenerator(qrRepo::RepoApi const &api)
    : mApi(api)
{
}

QHash<Id, QString> EditorGenerator::getMetamodelList()
{
    Id repoId = Id::rootId();

    IdList const metamodels = mApi.children(repoId);
    QHash<Id, QString> metamodelList;

    foreach (Id const key, metamodels) {
        QString const objectType = mApi.typeName(key);
        if (objectType == "MetamodelDiagram") {
            QString name = mApi.stringProperty(key, "name of the directory");
            if (!name.isEmpty())
                metamodelList.insert(key, name);
            else
                mErrorReporter.addWarning("no the name of the directory", key);
        }
    }
    return metamodelList;
}

gui::ErrorReporter &EditorGenerator::generateEditor(Id const metamodelId, const QString &pathToFile)
{
    QString includeProList;
    QFileInfo fileName(pathToFile);
    QString baseName = fileName.baseName();

    QDomElement metamodel = mDocument.createElement("metamodel");
    metamodel.setAttribute("xmlns", "http://schema.real.com/schema/");
    mDocument.appendChild(metamodel);

    QString const includeFile = mApi.stringProperty(metamodelId, "include");
    QStringList const includeList = includeFile.split(", ", QString::SkipEmptyParts);
    foreach (QString const name, includeList) {
        includeProList += " " + name;
        QDomElement include = mDocument.createElement("include");
        QDomText value = mDocument.createTextNode(name);
        include.appendChild(value);
        metamodel.appendChild(include);
    }

    QDomElement nameSpace = mDocument.createElement("namespace");
    QDomText nameSpaceName = mDocument.createTextNode("UML 2.0");
    nameSpace.appendChild(nameSpaceName);
    metamodel.appendChild(nameSpace);

    createDiagrams (metamodel, metamodelId);

    OutFile outpro(pathToFile + ".pro");
    outpro() << QString("QREAL_XML = %1\n").arg(baseName + ".xml");
    if (includeProList != "")
        outpro() << QString("QREAL_XML_DEPENDS = %1\n").arg(includeProList);
    outpro() << QString ("QREAL_EDITOR_NAME = %1\n").arg(baseName);
    outpro() << "\n";
    outpro() << "include (../editorsCommon.pri)";

    OutFile outxml(pathToFile + ".xml");
    outxml() << "<?xml version='1.0' encoding='utf-8'?>\n";
    outxml() << mDocument.toString(4);
    mDocument.clear();
    return mErrorReporter;

}

void EditorGenerator::createDiagrams(QDomElement &parent, const Id &id)
{
    IdList rootElements = mApi.children(id);
    foreach (Id const typeElement, rootElements) {
        QString const objectType = mApi.typeName(typeElement);
        if (objectType == "MetaEditorDiagramNode") {
            QDomElement diagram = mDocument.createElement("diagram");
            ensureCorrectness(typeElement, diagram, "name", mApi.name(typeElement));
            ensureCorrectness(typeElement, diagram, "displayedName", mApi.stringProperty(typeElement, "displayedName"));
            ensureCorrectness(typeElement, diagram, "nodeName", mApi.stringProperty(typeElement, "nodeName"));
            parent.appendChild(diagram);

            serializeObjects(diagram, typeElement);
            mElements.clear();
        }
        else if (objectType == "Listener") {
            QDomElement listener = mDocument.createElement("listener");
            ensureCorrectness(typeElement, listener, "class", mApi.stringProperty(typeElement, "class"));
            ensureCorrectness(typeElement, listener, "file", mApi.stringProperty(typeElement, "file"));
            parent.appendChild(listener);
        }
    }
}

void EditorGenerator::serializeObjects(QDomElement &parent, Id const &idParent)
{
    IdList const childElems = mApi.children(idParent);
    mElements = childElems;
    mDiagramName = mApi.name(idParent);

    QDomElement tagNonGraphic = mDocument.createElement("nonGraphicTypes");

    foreach (Id const id, childElems) {
        if (idParent != Id::rootId()) {
            QString const objectType = mApi.typeName(id);
            if (objectType == "MetaEntityEnum")
                createEnum(tagNonGraphic, id);
        }
    }

    if (!tagNonGraphic.childNodes().isEmpty())
        parent.appendChild(tagNonGraphic);

    QDomElement tagGraphic = mDocument.createElement("graphicTypes");
    parent.appendChild(tagGraphic);

    foreach (Id const id, childElems) {
        if (idParent != Id::rootId()) {
            QString const objectType = mApi.typeName(id);
            if (objectType == "MetaEntityImport")
                createImport(tagGraphic, id);
            if (objectType == "MetaEntityNode")
                createNode (tagGraphic, id);
            if (objectType == "MetaEntityEdge")
                createEdge (tagGraphic, id);
        }
    }
}

void EditorGenerator::createImport(QDomElement &parent, const Id &id)
{
    QDomElement import = mDocument.createElement("import");
    if ((mApi.stringProperty(id, "importedFrom") != "") && (mApi.name(id) != ""))
        ensureCorrectness(id, import, "name", mApi.stringProperty(id, "importedFrom") + "::" + mApi.name(id));
    else {
        mErrorReporter.addWarning(QString ("not filled name/importedFrom"), id);
        import.setAttribute("name", "");
    }
    ensureCorrectness(id, import, "displayedName", mApi.stringProperty(id, "displayedName"));
    ensureCorrectness(id, import, "as", mApi.stringProperty(id, "as"));
    parent.appendChild(import);
}

void EditorGenerator::createNode(QDomElement &parent, Id const &id)
{
    QDomElement node = mDocument.createElement("node");
    ensureCorrectness(id, node, "name", mApi.name(id));
    ensureCorrectness(id, node, "displayedName", mApi.stringProperty(id, "displayedName"));
    if (mApi.stringProperty(id, "path") != "")
        node.setAttribute("path", mApi.stringProperty(id, "path"));
    parent.appendChild(node);

    QDomDocument graphics;
    graphics.setContent(mApi.stringProperty(id, "shape"));
    node.appendChild(graphics);

    QDomElement logic = mDocument.createElement("logic");
    node.appendChild(logic);

    setContainer(logic, id);
    setUsages(logic, id);
    setConnections(logic, id);
    setProperties(logic, id);
    setPin(logic, id);
    setAction(logic, id);
    setGeneralization(logic, id);
    setContextMenuFields(logic, id);
}

void EditorGenerator::createEdge(QDomElement &parent, Id const &id)
{
    QDomElement edge = mDocument.createElement("edge");
    ensureCorrectness(id, edge, "name", mApi.name(id));
    ensureCorrectness(id, edge, "displayedName", mApi.stringProperty(id, "displayedName"));
    parent.appendChild(edge);

    if (mApi.stringProperty(id, "lineType") != "") {
        QDomElement graphics = mDocument.createElement("graphics");
        edge.appendChild(graphics);
        QDomElement lineType = mDocument.createElement("lineType");
        ensureCorrectness(id, lineType, "type", mApi.stringProperty(id, "lineType"));
        graphics.appendChild(lineType);
    }

    QDomElement logic = mDocument.createElement("logic");
    edge.appendChild(logic);

    setAssociations(logic, id);
    setPossibleEdges(logic, id);
    setProperties(logic, id);
    setGeneralization(logic, id);
}

void EditorGenerator::createEnum(QDomElement &parent,Id const &id)
{
    QDomElement enumElement = mDocument.createElement("enum");
    ensureCorrectness(id, enumElement, "name", mApi.name(id));
    ensureCorrectness(id, enumElement, "displayedName", mApi.stringProperty(id, "displayedName"));
    parent.appendChild(enumElement);

    setValues(enumElement, id);
}

void EditorGenerator::setGeneralization(QDomElement &parent, const Id &id)
{
    QDomElement generalizations = mDocument.createElement("generalizations");

    IdList const inLinks = mApi.incomingLinks(id);

    foreach (Id const inLink, inLinks) {
        if (mApi.typeName(inLink) == "Inheritance") {
            Id const parentId = mApi.from(inLink);
            if ((mApi.typeName(parentId) == "MetaEntityImport") || (mApi.typeName(parentId) == "MetaEntityNode") || (mApi.typeName(parentId) == "MetaEntityEdge")) {
                QDomElement generalization = mDocument.createElement("parent");
                ensureCorrectness(parentId, generalization, "parentName", mApi.stringProperty(parentId, "name"));
                generalizations.appendChild(generalization);
            }
        }
    }
    if (!generalizations.childNodes().isEmpty())
        parent.appendChild(generalizations);

    /*IdList parents = mApi.children(id);
    foreach (Id const id, parents) {
        QString objectType = mApi.typeName(id);
        if (objectType == "MetaEntityParent") {
            QDomElement parent = mDocument.createElement("parent");
            ensureCorrectness(id, parent, "parentName", mApi.stringProperty(id, "name"));
            generalizations.appendChild(parent);
        }
    }

    if (!generalizations.childNodes().isEmpty())
        parent.appendChild(generalizations);*/
}

void EditorGenerator::setProperties(QDomElement &parent,Id const &id)
{
    QDomElement tagProperties = mDocument.createElement("properties");
    IdList const childElems = mApi.children(id);

    foreach (Id const idChild, childElems)
        if (idChild != Id::rootId()) {
        QString const objectType = mApi.typeName(idChild);
        if (objectType == "MetaEntity_Attribute"){
            QDomElement property = mDocument.createElement("property");
            ensureCorrectness(idChild, property, "type", mApi.stringProperty(idChild, "attributeType"));
            ensureCorrectness(idChild, property, "name", mApi.name(idChild));
            if (mApi.stringProperty(idChild, "defaultValue") != "") {
                QDomElement defaultTag = mDocument.createElement("default");
                QDomText value = mDocument.createTextNode(mApi.stringProperty(idChild, "defaultValue"));
                defaultTag.appendChild(value);
                property.appendChild(defaultTag);
            }
            tagProperties.appendChild(property);
        }
    }

    if (!tagProperties.childNodes().isEmpty())
        parent.appendChild(tagProperties);
}

void EditorGenerator::setContextMenuFields(QDomElement &parent, const Id &id)
{
    QDomElement fields = mDocument.createElement("bonusContextMenuFields");
    IdList const childElems = mApi.children(id);

    foreach (Id const idChild, childElems)
        if (idChild != Id::rootId()) {
        QString const objectType = mApi.typeName(idChild);
        if (objectType == "MetaEntityContextMenuField"){
            QDomElement field = mDocument.createElement("field");
            ensureCorrectness(idChild, field, "name", mApi.name(idChild));
            fields.appendChild(field);
        }
    }

    if (!fields.childNodes().isEmpty())
        parent.appendChild(fields);
}

void EditorGenerator::setValues(QDomElement &parent, const Id &id)
{
    IdList childElems = mApi.children(id);

    foreach (Id const idChild, childElems) {
        if (idChild != Id::rootId()) {
            QDomElement valueTag = mDocument.createElement("value");
            QDomText value = mDocument.createTextNode(mApi.stringProperty(idChild, "valueName"));
            valueTag.appendChild(value);
            parent.appendChild(valueTag);
        }
    }
}

void EditorGenerator::setAssociations(QDomElement &parent, const Id &id)
{
    IdList const childElems = mApi.children(id);

    foreach (Id const idChild, childElems) {
        QString const objectType = mApi.typeName(idChild);
        if (objectType == "MetaEntityAssociation") {
            QDomElement associationTag = mDocument.createElement("associations");
            ensureCorrectness(idChild, associationTag, "beginType", mApi.stringProperty(idChild, "beginType"));
            ensureCorrectness(idChild, associationTag, "endType", mApi.stringProperty(idChild, "endType"));
            parent.appendChild(associationTag);

            QDomElement association = mDocument.createElement("association");
            ensureCorrectness(idChild, association, "beginName", mApi.stringProperty(idChild, "beginName"));
            ensureCorrectness(idChild, association, "endName", mApi.stringProperty(idChild, "endName"));
            associationTag.appendChild(association);
        }
    }
}

void EditorGenerator::setUsages(QDomElement &parent, const Id &id)
{
    newSetConnections(parent, id, "usages", "usage", "MetaEntityUsage");
}

void EditorGenerator::setConnections(QDomElement &parent, const Id &id)
{
    newSetConnections(parent, id, "connections", "connection", "MetaEntityConnection");
}

void EditorGenerator::newSetConnections(QDomElement &parent, const Id &id,
        QString const &commonTagName, QString const &internalTagName, QString const &typeName)
{
    IdList const childElems = mApi.children(id);

    QDomElement connectionsTag = mDocument.createElement(commonTagName);

    foreach (Id const idChild, childElems) {
        QString const objectType = mApi.typeName(idChild);
        if (objectType == typeName) {
            QDomElement connection = mDocument.createElement(internalTagName);
            ensureCorrectness(idChild, connection,"type", mApi.stringProperty(idChild, "type"));
            connectionsTag.appendChild(connection);
        }
    }

    if (!connectionsTag.childNodes().isEmpty())
        parent.appendChild(connectionsTag);
}

void EditorGenerator::setPossibleEdges(QDomElement &parent, const Id &id)
{
    IdList const childElems = mApi.children(id);

    QDomElement possibleEdges = mDocument.createElement("possibleEdges");

    foreach (Id const idChild, childElems) {
        QString const objectType = mApi.typeName(idChild);
        if (objectType == "MetaEntityPossibleEdge") {
            QDomElement possibleEdge = mDocument.createElement("possibleEdge");
            possibleEdges.appendChild(possibleEdge);
            ensureCorrectness(idChild, possibleEdge, "beginName", mApi.stringProperty(idChild, "beginName"));
            ensureCorrectness(idChild, possibleEdge, "endName", mApi.stringProperty(idChild, "endName"));
            ensureCorrectness(idChild, possibleEdge, "directed", mApi.stringProperty(idChild, "directed"));
        }
    }

    if (!possibleEdges.childNodes().isEmpty())
        parent.appendChild(possibleEdges);
}

void EditorGenerator::setPin(QDomElement &parent, const Id &id)
{
    setStatusElement(parent, id, "pin", "isPin");
}

void EditorGenerator::setAction(QDomElement &parent, const Id &id)
{
    setStatusElement(parent, id, "action", "isAction");
}

void EditorGenerator::setStatusElement(QDomElement &parent, const Id &id, const QString &tagName, const QString &propertyName)
{
    if (mApi.stringProperty(id, propertyName) == "true") {
        QDomElement statusElement = mDocument.createElement(tagName);
        parent.appendChild(statusElement);
    }
}

void EditorGenerator::setContainer(QDomElement &parent, const Id &id)
{
    /*if (!mApi.hasProperty(id, "container") || mApi.stringProperty(id, "container").isEmpty())
        return;

    QDomElement container = mDocument.createElement("container");
    parent.appendChild(container);

    QStringList const types = mApi.stringProperty(id, "container").split(',');
    foreach (QString type, types) {
        QDomElement contains = mDocument.createElement("contains");
        ensureCorrectness(id, contains, "type", type);
        container.appendChild(contains);
    }*/
    QDomElement container = mDocument.createElement("container");
    parent.appendChild(container);

    IdList inLinks = mApi.outgoingLinks(id);
    foreach (Id const inLink, inLinks) {
        if (mApi.typeName(inLink) == "Container") {
            Id const elementId = mApi.to(inLink);
            QString const typeName = mApi.typeName(elementId);
            if (typeName == "MetaEntityNode") {
                QDomElement contains = mDocument.createElement("contains");
                ensureCorrectness(elementId, contains, "type", mApi.name(elementId));
                container.appendChild(contains);
            }
            else if (typeName == "MetaEntityImport") {
                QDomElement contains = mDocument.createElement("contains");
                ensureCorrectness(elementId, contains, "type", mApi.stringProperty(elementId, "importedFrom") + "::" + mApi.name(elementId));
                container.appendChild(contains);
            }
        }
    }

    setContainerProperties(container, id);
}

void EditorGenerator::setContainerProperties(QDomElement &parent, const Id &id)
{
    IdList elements = mApi.children(id);

    foreach (Id const idChild, elements) {
        if (mApi.typeName(idChild) == "MetaEntityPropertiesAsContainer") {
            QDomElement properties = mDocument.createElement("properties");
            parent.appendChild(properties);

            setSizesForContainer("forestalling", properties, idChild);
            setSizesForContainer("childrenForestalling", properties, idChild);

            setBoolValuesForContainer("sortContainer", properties, idChild);
            setBoolValuesForContainer("banChildrenMove", properties, idChild);
            setBoolValuesForContainer("minimizeToChildren", properties, idChild);
            setBoolValuesForContainer("maximizeChildren", properties, idChild);
        }
    }
}

void EditorGenerator::setSizesForContainer(const QString &propertyName,QDomElement &properties, const Id &id)
{
    if (mApi.stringProperty(id, propertyName + "Size") != "") {
        QDomElement property = mDocument.createElement(propertyName);
        properties.appendChild(property);
        property.setAttribute("size", mApi.stringProperty(id, propertyName + "Size"));
    }
}

void EditorGenerator::setBoolValuesForContainer(const QString &propertyName,QDomElement &properties, const Id &id)
{
    if (mApi.stringProperty(id, propertyName) == "true") {
        QDomElement property = mDocument.createElement(propertyName);
        properties.appendChild(property);
    }
}

void EditorGenerator::ensureCorrectness(const Id &id, QDomElement element, const QString &tagName, const QString &value)
{
    QString tag = tagName;
    if ((value == "") && ((tag == "displayedName")))
        return;
    if (value == "") {
        mErrorReporter.addWarning(QString ("not filled %1\n").arg(tagName), id);
        element.setAttribute(tagName, "");
    }
    else if (tag == "name") {
        QRegExp patten;
        patten.setPattern("[A-Za-z_]+([A-Za-z_0-9 :]*)");
        if (patten.exactMatch(value))
            element.setAttribute(tagName, value);
        else {
            mErrorReporter.addWarning("wrong name\n",id);
            element.setAttribute(tagName, value);
        }
    }
    else element.setAttribute(tagName, value);
}

