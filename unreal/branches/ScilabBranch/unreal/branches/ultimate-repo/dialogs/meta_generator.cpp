#include "meta_generator.h"
#include "ui_meta_generator.h"
#include <QDomDocument>
#include <QFile>

using namespace qReal;

QString meta_generator::getName(QString IdType) {
    QString data = client->getObjectData(IdType);
    QStringList dataList = data.split("	");
    return dataList.at(1);
}
QString meta_generator::getType(QString IdType) {
    QString data = client->getObjectData(IdType);
    QStringList dataList = data.split("	");
    return dataList.at(2);
}
QString meta_generator::getOtherEntityFromLink(QString LinkId, QString firstNode) {
    QString data = client->getObjectsByLink(LinkId);
    QStringList dataList = data.split("\t");
    if (dataList.size() - 1 == 4)
        for (int i = 0; i< (dataList.size() - 1) / 2; i++){
               //here we are asking things about link
            if (dataList.at(i*2+1) !=firstNode)
                return dataList.at(i*2 +1);
        }
    return "";
}


// Функция создающая сущности и отношения в редакторе
QDomElement meta_generator::createNode(QDomDocument doc, QString nodeId, QString prefix = "tmp") {
    QString stringType = client->getPropValue(nodeId,"entity_type");
    if (stringType != "node" && stringType !="edge") stringType = "node";
    QString postprefix = (stringType == "node")? ("n"):("e");
    QString nameString = getName(nodeId);
    QString idString = prefix + postprefix + nameString;

    QDomElement resultElement = doc.createElement(stringType);
    QDomElement graphElement = doc.createElement("graphics");
    QDomElement viewElement = doc.createElement("view");
    graphElement.appendChild(viewElement);
    resultElement.appendChild(graphElement);
    QDomElement domLogicElement = doc.createElement("logic");
    resultElement.appendChild(domLogicElement);
    QDomElement domPropElement = doc.createElement("properties");
    QDomElement domGeneElement = doc.createElement("generalizations");



    resultElement.setAttribute("id", idString);
    resultElement.setAttribute("name", nameString);

    if (client->getPropValue(nodeId, "line_type") != "" && postprefix == "e") {
        QDomElement lineType = doc.createElement("line_type");
        lineType.setAttribute("type", client->getPropValue(nodeId, "line_type"));
        viewElement.appendChild(lineType);
    }
    if (postprefix == "n") {
        QDomElement picture = doc.createElement("picture");
        picture.setAttribute("sizex", "31");
        picture.setAttribute("sizey", "100");
        viewElement.appendChild(picture);
    }

    // here links should be proceded
    QString data = client->getLinksByObject(nodeId, 2);
    if (data.length() > 2) {
        QStringList dataList = data.split("\t");
        for (int i = 0; i< dataList.size() - 1; i++){
           //here we are asking things about link

            QString otherNode = getOtherEntityFromLink(dataList.at(i), nodeId);
            if (getType(otherNode) == "mednMetaEntityAttribute") {
                QDomElement domProp = doc.createElement("property");
                domPropElement.appendChild(domProp);
                domProp.setAttribute("name", getName(otherNode));
                domProp.setAttribute("type", ((client->getPropValue(otherNode,"attributeType") == "")? \
                                              ("string"):(client->getPropValue(otherNode,"attributeType"))));
                continue;
            }
            if (getType(otherNode) == "mednMetaEntity") {
                QDomElement domGene = doc.createElement("parent");
                domGeneElement.appendChild(domGene);
                domGene.setAttribute("parent_id", prefix + \
                                     ((client->getPropValue(dataList.at(i),"entity_type") == "node") ? ("n"):("e")) + \
                                     getName(dataList.at(i)));
            } else {
                QDomElement domGene = doc.createElement("parent");
                domGeneElement.appendChild(domGene);
                domGene.setAttribute("parent_id", getType(otherNode));
            }
        }
    }
    domLogicElement.appendChild(domPropElement);
    domLogicElement.appendChild(domGeneElement);


    return resultElement;
}

// Функция создает и сохраняет редактор редактор
void meta_generator::createMetaEditor(QString EditorIdType) {
    QString editorTitle = getName(EditorIdType); // getNameFromId("");
    QString includeString = "kernel_metamodel";
    QString entityPrefix = (client->getPropValue(EditorIdType,"prefix_name") == "") ? ("tmp"):(client->getPropValue(EditorIdType,"prefix_name"));

    QDomDocument doc(editorTitle);


    QDomElement domMainElement = doc.createElement("metamodel");
    domMainElement.setAttribute("xmlns","http://schema.real.com/schema/");
    doc.appendChild(domMainElement);

    QDomElement domIncludeEl = doc.createElement("include");
    domIncludeEl.appendChild(doc.createTextNode(includeString));

    QDomElement domNamespaceEl = doc.createElement("namespace");
    domNamespaceEl.appendChild(doc.createTextNode(editorTitle));
    QDomElement domEditorEl = doc.createElement("editor");
    domMainElement.appendChild(domIncludeEl);
    domMainElement.appendChild(domNamespaceEl);
    domMainElement.appendChild(domEditorEl);


    domEditorEl.setAttribute("name", editorTitle);

    QString data = client->getChildren(EditorIdType);
    if (data.length() > 2) {
        QStringList dataList = data.split("\t");
        for (int i = 0; i< dataList.size() - 1; i++){
            if (getType(dataList.at(i)) == "mednMetaEntity") {
                QDomElement domNewNode = createNode(doc, dataList.at(i), entityPrefix);
                domEditorEl.appendChild(domNewNode);
            }
        }
    }


    QFile file(editorTitle + ".xml");
    if (file.open(QIODevice::WriteOnly))  {
        QTextStream(&file) << "<?xml version='1.0' encoding='utf-8'?>\n";
        QTextStream(&file) << doc.toString();
        file.close();
    }

    return;
}

void meta_generator::createEditor() {
    QListWidgetItem* editorItem = ui->listWidget->currentItem();
    QString editorItemText = editorItem->text();
    QString editorId = editorItemText.split("\t").at(0);
    if (editorId != "")
        createMetaEditor(editorId);
}



meta_generator::meta_generator()
    : ui(new Ui::meta_generatorClass)
{
    ui->setupUi(this);
    QString addr = "127.0.0.1";
    int port = 6666;
    client = new RealRepoClient(addr, port);
  //  ui->label->setText(QString::number(client->state()));
 //   ui->plainTextEdit->setPlainText(client->getObjectData("889") + "      " + client->getChildren("889") \
  //                                  + client->getPropValue("920", "name") \
   //                                 + client->getChildren("919"));
  //  ui->plainTextEdit->setPlainText(client->getObjectData("889")+ "      " \
   //                                 + client->getLinksByObject("923", 2) + "    " \
   //                                 + client->getPropValue("923","entity_type") + "    " \
     //                               + client->getObjectsByLink("892"));
   // ui->plainTextEdit->setPlainText(client->getLinksByObject("891", 2));

    QString data = client->getObjectsByType("mednMetaEditor");

    QStringList dataList = data.split("\t");
    for (int i = 0; i< dataList.size(); i++){
        if (getType(dataList.at(i)) == "mednMetaEditor") {
            ui->listWidget->addItem(dataList.at(i) + "\t" + getName(dataList.at(i)));
        }
    }
    QObject::connect(ui->buttonMakeEditor, SIGNAL(clicked()), this, SLOT(createEditor()));
}




meta_generator::~meta_generator()
{
    delete ui;
}
