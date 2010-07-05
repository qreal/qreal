#include <QtCore/QCoreApplication>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QMap>

void bypass(QDomNode n, QDomDocument &doc){
    QDomElement e = n.toElement();
    if (e.isNull())
        return;
    else
        if(e.tagName() == "graph_types")
            e.setTagName("graphicTypes");
    else
        if(e.tagName() == "line_type")
            e.setTagName("lineType");
    else
        if(e.tagName() == "repo_info"){
        e.setTagName("labels");
        QDomNode childRepo = n.firstChild();
        QList<QString> listAttrRepo;
        QList<QString> listAttrText;
        while(!childRepo.isNull()){
            QDomNode childChildRepo = childRepo.firstChild();
            while(!childChildRepo.isNull()){
                QDomElement echildChildRepo = childChildRepo.toElement();
                if(echildChildRepo.tagName() == "html:text_from_repo"){
                    listAttrRepo.append(echildChildRepo.attribute("name"));
                }
                if(echildChildRepo.tagName() == "html:text"){
                    QDomNode childChildChildRepo = childChildRepo.firstChild();
                    listAttrText.append(childChildChildRepo.nodeValue());
                }
                childChildRepo = childChildRepo.firstChild();
            }
            childRepo = childRepo.nextSibling();
        }
        childRepo = n.firstChild();
        while(!childRepo.isNull()) {
            QDomNode childRepoSibling = childRepo.nextSibling();
            n.removeChild(childRepo);
            childRepo = childRepoSibling;
        }
        while(!listAttrRepo.isEmpty()){
            QDomElement eNewChildRepo = doc.createElement("label");
            eNewChildRepo.setAttribute("x", 0);
            eNewChildRepo.setAttribute("y", 0);
            eNewChildRepo.setAttribute("textBinded", listAttrRepo.takeFirst());
            n.appendChild(eNewChildRepo);
        }
        while(!listAttrText.isEmpty()){
            QDomElement eNewChildRepo = doc.createElement("label");
            eNewChildRepo.setAttribute("x", 0);
            eNewChildRepo.setAttribute("y", 0);
            eNewChildRepo.setAttribute("text", listAttrText.takeFirst());
            n.appendChild(eNewChildRepo);
        }
    }else
        if(e.tagName() == "line_port")
            e.setTagName("linePort");
    else
        if(e.tagName() == "view" ){
        QDomNode nParent = n.parentNode();
        QDomNode n1 = n.cloneNode(true);
        nParent.removeChild(n);
        QDomNode i = n1.firstChild();
        while(!i.isNull()){
            QDomNode iSibling = i.nextSibling();
            nParent.appendChild(i);
            i = iSibling;
        }
        n = nParent;
    }
    else
        if(e.tagName() == "generalization"){
        QDomNode childGen = n.firstChild();
        while(!childGen.isNull()) {
            QDomElement echildGen = childGen.toElement();
            QString attr = echildGen.attribute("parent_id");
            echildGen.removeAttribute("parent_id");
            echildGen.setAttribute("parentName", attr);
            childGen = childGen.nextSibling();
        }
        QDomNode nParent = n.parentNode();
        QDomNode n1 = n.cloneNode(true);
        nParent.removeChild(n);
        QDomNode i = n1.firstChild();
        while(!i.isNull()){
            QDomNode iSibling = i.nextSibling();
            nParent.appendChild(i);
            i = iSibling;
        }
        n = nParent;
    }else
        if(e.tagName() == "edge" || e.tagName() == "node" || e.tagName() == "enum"){
        QDomElement en = n.toElement();
        QString attrId = en.attribute("id");
        QString attrName = en.attribute("name");
        en.removeAttribute("id");
        en.removeAttribute("name");
        en.setAttribute("name", attrId);
        en.setAttribute("displayedName", attrName);
    }else
        if(e.tagName() == "include"){
        QDomNode childInc = n.firstChild();
        if(childInc.isText()){
            QString existText = childInc.nodeValue();
            QString newText = "../commonXml/" + existText + ".xml";
            childInc.setNodeValue(newText);
        }
    }else
        if(e.tagName() == "containers"){
        e.setTagName("container");
        QDomNode childCont = n.firstChild();
        QList<QDomElement> list;
        bool signBy = false;
        while(!childCont.isNull()) {
            QDomNode childChildCont = childCont.firstChild();
            QDomElement eChildChildCont = childChildCont.toElement();
            if(eChildChildCont.tagName() == "contained_by"){
                childCont.removeChild(childChildCont);
                signBy = true;
            }else{
                QString attr = eChildChildCont.attribute("idref");
                childCont.removeChild(childChildCont);
                QDomElement enewChildChildCont = doc.createElement("contains");
                enewChildChildCont.setAttribute("type", attr);
                list.append(enewChildChildCont);
                childCont = childCont.nextSibling();}
        }
        childCont = n.firstChild();
        while(!childCont.isNull()) {
            QDomNode childContSibling = childCont.nextSibling();
            n.removeChild(childCont);
            childCont = childContSibling;
        }
        while(!list.isEmpty() && !signBy) {
            n.appendChild(list.takeFirst());
        }
        if(!e.hasChildNodes()){
            QDomNode nSibling = n.nextSibling();
            QDomNode nParent = n.parentNode();
            nParent.removeChild(n);
            n = nSibling;
        }

    }else
        if(e.tagName() == "associations"){
        QDomNode childAsso = n.firstChild();
        while(!childAsso.isNull()) {
            QDomNode childAssoSibling = childAsso.nextSibling();
            n.removeChild(childAsso);
            childAsso = childAssoSibling;
        }
    }else
        if(e.tagName() == "end_type"){
        QString attrEndType = e.attribute("type");
        QDomNode nParent = n.parentNode();
        QDomNode nSibling = n.nextSibling();
        nParent.removeChild(n);
        n = nSibling;
        QDomNode nParentParent = nParent.parentNode();
        QDomElement enParentParent = nParentParent.toElement();
        QDomNodeList listLog = enParentParent.elementsByTagName("logic");
        QDomElement eParentLog = listLog.item(0).toElement();
        QDomNodeList listAsso = eParentLog.elementsByTagName("associations");
        listAsso.item(0).toElement().setAttribute("endType", attrEndType);
    }else
        if(e.tagName() == "begin_type"){
        QString attrBeginType = e.attribute("type");
        QDomNode nParent = n.parentNode();
        QDomNode nSibling = n.nextSibling();
        nParent.removeChild(n);
        n = nSibling;
        QDomNode nParentParent = nParent.parentNode();
        QDomElement enParentParent = nParentParent.toElement();
        QDomNodeList listLog = enParentParent.elementsByTagName("logic");
        QDomElement eParentLog = listLog.item(0).toElement();
        QDomNodeList listAsso = eParentLog.elementsByTagName("associations");
        listAsso.item(0).toElement().setAttribute("beginType", attrBeginType);
    }

    QDomNode child = n.firstChild();
    while(!child.isNull()) {
        QDomNode childSibling = child.nextSibling();
        bypass(child, doc);
        child = childSibling;
    }
}

void getId(QDomNode n, QDomDocument &doc, QDomDocument &docId, QList<QString> &listId){
    QDomElement e = n.toElement();
    if (e.isNull())
        return;
    else if(e.tagName() == "parent") {
        QString attrForId = e.attribute("parentName");
        if(!listId.contains(attrForId)){
            listId.append(attrForId);
            QDomElement newId = docId.createElement("id");
            docId.appendChild(newId);
            newId.setAttribute("was", attrForId);
            newId.setAttribute("now", "");
        }
    }

    QDomNode child = n.firstChild();
    while(!child.isNull()) {
        QDomNode childSibling = child.nextSibling();
        getId(child, doc, docId, listId);
        child = childSibling;
    }
}

void map(QDomNode n, QMap<QString, QString> &mapId){
    QDomElement e = n.toElement();
    mapId.insert(e.attribute("was"), e.attribute("now"));
    QDomNode neighbor = n.nextSibling();
    while(!neighbor.isNull()) {
        QDomNode neighborSibling = neighbor.nextSibling();
        map(neighbor, mapId);
        neighbor = neighborSibling;
    }
}

void changeId(QDomNode n, QMap<QString, QString> &mapId){
    QDomElement e = n.toElement();
    if (e.isNull())
        return;
    else if(e.tagName() == "parent") {
        e.setAttribute("parentName", mapId[e.attribute("parentName")]);
    }
    QDomNode child = n.firstChild();
    while(!child.isNull()) {
        QDomNode childSibling = child.nextSibling();
        changeId(child, mapId);
        child = childSibling;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QDomDocument doc;
    QDomDocument docId;
    QFile file(a.arguments().at(1));
    if (!file.open(QIODevice::ReadOnly)) {
        return -1;
    }
    if (!doc.setContent(&file)) {
        file.close();
        return -1;
    }
    file.close();
    QDomElement docRoot = doc.documentElement();
    QList<QString> listId;
    QMap<QString, QString> mapId;
    bypass(docRoot, doc);
    getId(docRoot, doc, docId, listId);
    QDomElement docRootId = docId.documentElement();
    map(docRootId, mapId);
    docRoot = doc.documentElement();
    changeId(docRoot, mapId);
    //qDebug() << doc.toString(4);

    QString filename = "outputFile";
    QFile fileOut( filename );
    if (fileOut.open(QIODevice::WriteOnly))
    {
        QTextStream stream( &fileOut );
        stream << doc.toString(4) << endl;
    }
    fileOut.close();

    QFile fileId( "idCollector" );
    if (fileId.open(QIODevice::WriteOnly))
    {
        QTextStream stream( &fileId );
        stream << docId.toString(4) << endl;
    }
    fileId.close();
    return 0;
}
