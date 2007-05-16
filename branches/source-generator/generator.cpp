#include "generator.h"

Generator::Generator( QStringList files ){

    // inits
    resources = "<!DOCTYPE RCC><RCC version=\"1.0\">\n<qresource>\n";
    res = "\t<file>%1</file>\n";

    untitled = 0;
    objectsCount = 0;

    // creating directory for generated stuff
    dir.cd(".");
    dir.mkdir("generated");
    dir.cd("generated");
    // parse all files
    for (int i=0; i<files.size(); i++){
        qDebug() << "processing file " << files.at(i);
        parseFile(files.at(i));
    } 
    // propagate properties and other useful things
    propagateAll();

    // generate all the stuff needed
    genEnums();
    genSQLScripts();
    genClasses();
    genFactory();
    genRealRepoInfo();
    genEdgesFunction();

    // write the resource file
    QFile file("generated/real_dynamic.qrc");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
    resources += "</qresource>\n</RCC>";
    
    out << resources;

    file.close();
    
    qDebug() << "done";
}

Generator::~Generator(){
   // TODO: delete objects and edges
}

void Generator::parseFile( QString filename ){

    QDomDocument *doc = new QDomDocument("+1"); // :)
    QFile file(filename);
    if( !file.open(QIODevice::ReadOnly)){
        qDebug() << "incorrect filename " << filename;
        return;
    }
    if( !doc->setContent(&file)){
        file.close();
        qDebug() << "cannot set qdomdocument's content in " << filename;
        return;
    }
    file.close();

    Category* cat = new Category();
    cat->name = doc->elementsByTagName("diagram").at(0).toElement().attribute("name");
    categories << cat;

    // I. parsing enums
    QDomNodeList enumList = doc->elementsByTagName("enumType"); 
    for( int i=0; i < (int) enumList.length(); i++){
        parseEnum( enumList.at(i) );
    }

    // II. parsing nodes
    QDomNodeList nodeList = doc->elementsByTagName("node"); 
    for( int i=0; i < (int) nodeList.length(); i++ ){
        parseNode( nodeList.at(i) );
    }

    // III. parsing edges
    QDomNodeList edgeList = doc->elementsByTagName("edge");
    for( int i=0; i < (int) edgeList.length(); i++ ){
        parseEdge( edgeList.at(i) );
    }

}

void Generator::parseEnum( QDomNode dnode ){
    
    QDomElement cur = dnode.toElement();
    QStringList values;
    QString name  = cur.attribute("id");
    QString qname = cur.attribute("name");
    QDomNodeList vals = cur.elementsByTagName("enumValue");
    values << qname;
    for( int k = 0; k < (int) vals.length(); k++ )
        values << vals.at(k).toElement().text();
    enumerations.insert(name, values);

}

void Generator::parseNode( QDomNode dnode ){

    Node *cur = new Node();
    cur->id  = dnode.toElement().attribute("id");
    cur->name = dnode.toElement().attribute("name");
    QDomElement logic = dnode.firstChildElement("logic");
    bool isNode = true;
    
    parseGeneralizations( cur, logic );
    parseProperties( cur, logic );
    parseAssociations( cur, logic, isNode);
    parsePorts( cur, dnode );
    parseSVG( cur, dnode ); 

    cur->type = NODE;
    objects << cur;
    categories.at(categories.size()-1)->objects << objectsCount++;    
}

void Generator::parseEdge( QDomNode dnode ){

    Edge *cur = new Edge();
    cur->id  = dnode.toElement().attribute("id");
    cur->name = dnode.toElement().attribute("name");
    QDomElement logic = dnode.firstChildElement("logic");
    bool isNode = false;
    
    // quick hack to make these props be on top
    cur->properties << QPair<QString, QString>("from", "string");
    cur->properties << QPair<QString, QString>("to", "string");
    cur->properties << QPair<QString, QString>("fromPort", "string");
    cur->properties << QPair<QString, QString>("toPort", "string");

    parseGeneralizations( cur, logic ); 
    parseProperties( cur, logic );
    parseAssociations( cur, logic, isNode );

    cur->height = -1;
    cur->width = -1;
    cur->type = EDGE;
    edges << cur;    
    objects << cur;  

    categories.at(categories.size()-1)->objects << objectsCount++;    
}

void Generator::parseGeneralizations( Entity* cur, QDomNode logic ){

    QDomNodeList gens = logic.firstChildElement("generalizations").toElement().elementsByTagName("generalization");
    // for each <generalization> tag
    for( int i=0; i < (int) gens.size(); i++ ){
        QString parentID = gens.at(i).firstChildElement("parent").attribute("parent_id");
        cur->addParent( parentID );
    } 
}

void Generator::parseProperties( Entity* cur, QDomNode logic ){

    QDomNodeList props = logic.toElement().elementsByTagName("properties");
    for( int i=0; i<props.size(); i++){  // for each <properties>...</properties> section
        QDomNodeList property = props.at(i).toElement().elementsByTagName("property");
        for( int j=0; j<property.size(); j++ ){   // for each <property ... > tag
                QDomNamedNodeMap attrs = property.at(j).attributes();
                QString name;
                QString type;
                if( !attrs.isEmpty() ){
                    name = attrs.namedItem("name").toAttr().value();
                    type = attrs.namedItem("type").toAttr().value();
                }
                else{
                    name = property.at(j).firstChildElement("name").text();
                    type = property.at(j).firstChildElement("type").text();
                }    
                if( type == "enum" ){
                    type = "enum " + property.at(j).firstChildElement("enum").attribute("idref");
                }
                if( type == "ref" ){
                    type = property.at(j).firstChildElement("ref").attribute("idref");
                } 
                cur->addProperty( name, type );
                // TODO: defaults and other missing property stuff support               
        }
    }    
}

void Generator::parsePorts( Node* cur, QDomNode dnode ){
    
    QDomNodeList ports = dnode.toElement().elementsByTagName("port");
    for( int i=0; i<ports.size(); i++ ){
        Port port;
        QString type = ports.at(i).toElement().attribute("type");
        port.type = type;
        if( type == "point"){
            port.vals << ports.at(i).toElement().attribute("x").toInt();
            port.vals << ports.at(i).toElement().attribute("y").toInt();
        }
        else if( type == "line" ){
            port.vals << ports.at(i).toElement().attribute("x1").toInt();
            port.vals << ports.at(i).toElement().attribute("x2").toInt();
            port.vals << ports.at(i).toElement().attribute("y1").toInt();
            port.vals << ports.at(i).toElement().attribute("y2").toInt();
        }
        else if( type == "ellipse" ){
            port.vals << ports.at(i).toElement().attribute("x").toInt();
            port.vals << ports.at(i).toElement().attribute("y").toInt();
            port.vals << ports.at(i).toElement().attribute("rx").toInt();
            port.vals << ports.at(i).toElement().attribute("ry").toInt();
        }
        cur->ports << port;
    }
}


void Generator::parseSVG( Entity* cur, QDomNode dnode ){

    QDomNodeList svg = dnode.toElement().elementsByTagName("svg:svg");
    if( !dir.exists("shapes") )
        dir.mkdir("shapes");

    if (!svg.isEmpty()){
        cur->height = svg.at(0).toElement().attribute("height").toInt();
        cur->width = svg.at(0).toElement().attribute("width").toInt();

        resources += res.arg("shapes/" + cur->id + "Class.svg");

        QFile file("generated/shapes/" + cur->id + "Class.svg");
        if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
            return;
        QTextStream stream(&file);
        svg.at(0).save(stream, 1);
        file.close();
    }
    else {
        cur->height = -1;
        cur->width = -1;
    }    
}

void Generator::parseAssociations( Entity *cur, QDomNode logic, bool isNode ){
    
    Edge* edge;
    QDomNodeList assocs = logic.toElement().elementsByTagName("association");
    if( assocs.size() == 0 )
        return;
    if( isNode ){
        edge = new Edge();
        edge->id = QString("untitledEdge_%1").arg(untitled);
        edge->name = QString("embedded edge #%1").arg(untitled);
        untitled++;
        edge->height = -1;
        edge->width = -1;
        edge->type = EDGE;
    }    
    else
        edge = (Edge*) cur;
    //TODO: multiple associations support
    Association* ass = new Association();
    for( int i=0; i < assocs.size(); i++){  
        QString role = assocs.at(i).toElement().attribute("id");
        QDomElement begin = assocs.at(i).firstChildElement("begin");
        QDomElement end = assocs.at(i).firstChildElement("end");
        if( begin != QDomElement() ){
            ass->from = begin.toElement().attribute("idref");
            ass->fromID = role;
        }    
        if( end != QDomElement() ){
            ass->to = end.toElement().attribute("idref");
            ass->toID = role;
        }   
    }
    edge->associations << ass;
    if( isNode ){
        objects << edge;
        edges << edge;
    }
}


void Generator::genEnums()
{
    if( !dir.exists("repo") )
        dir.mkdir("repo");
    QFile file("generated/repo/realreporoles.h");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
    
    int id = NUM;
    
    out << "#ifndef REALREPOROLES_H\n#define REALREPOROLES_H\n\n";
    
    QString tmp2 = "#include \"%1\"\n";

    out << "namespace UML {\n";
    out << "\tenum ElementTypes{\n";
    for( int i=0; i < objects.size(); i++ ){
        out << "\t\t" +  objects.at(i)->id + 
                        "=" + QString::number(id++);
        if( i!=objects.size()-1 )
            out << ",";
        out << "\n";
    }
    out << "\t};\n};\n\n";

    out << "namespace Unreal{\n";
    out << "\tenum Roles {\n"
           "\t\tPositionRole = Qt::UserRole + 64,   // Position on a diagram\n"
           "\t\tConfigurationRole,          // Configuration (e.g. for link)\n"
           "\t\tIdRole,\n"
           "\t\tTypeRole,\n"
           "\t\tUserRole = Qt::UserRole + 96        // First role available for other types\n\t};\n\n";
   
    for( int i=0; i < objects.size(); i++ ){
        out << "\tnamespace " + objects.at(i)->id + " {\n";
	out << "\t\tenum Roles {\n";
        for (int j=0; j<objects.at(i)->properties.size(); j++){
            out << "\t\t\t" + objects.at(i)->properties.at(j).first + "Role";
            if( !j )
                out << " = UserRole + 1";
            if( j != objects.at(i)->properties.size()-1)
                out << ",";
            out << "\n";
        }
	out << "\t\t};\n";
        out << "\t};\n\n";
   }
   
    out << "};\n\n";

    out << 
           "namespace Unreal {\n\t"
           "enum ClassRoles {\n\t\tFieldsRole = UserRole + 1,\n\t\tMethodsRole\n\t};\n\n\t"
           "enum LinkRoles {\n\t\tFromRole = UserRole + 1,\n\t\tToRole,\n\t\tFromPortRole,\n\t\tToPortRole\n\t};\n};\n";


    out << "#endif\n";
    
    file.close();
}

void Generator::genSQLScripts()
{
    QFile file("generated/repo/scripts.sql");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
   
    resources += res.arg("repo/scripts.sql");
   
    out << "CREATE TABLE nametable (\n"
            "\tid INTEGER PRIMARY KEY NOT NULL,\n"
            "\ttype MEDIUMINT NOT NULL,\n"
            "\tname VARCHAR(255),\n"
            "\tqualifiedName VARCHAR(255)\n"
            ");\n\n"
            "CREATE TABLE metatable (\n"
            "\tid INTEGER PRIMARY KEY NOT NULL,\n"
            "\tname VARCHAR(255),\n"
            "\tqualifiedName VARCHAR(255)\n"
            ");\n\n"
            "CREATE TABLE diagram (\n"
            "\tdiagram_id INTEGER PRIMARY KEY NOT NULL,\n"
            "\tel_id MEDIUMINT NOT NULL,\n"
            "\tx MEDIUMINT,\n"
            "\ty MEDIUMINT\n"
            ");\n\n";
                                                                                                                                            
    QString ins = "INSERT INTO `metatable` (id, name, qualifiedName) VALUES (%1, '%2', '%3');\n";
    QString inserts = "";
    
    
//    inserts += ins.arg(10).arg("Diagram");
    
    for (int i=0; i<objects.size(); i++){
        int j = i+NUM;
        inserts += ins.arg(j).arg(objects.at(i)->id).arg(objects.at(i)->name);
                

        out <<  "CREATE TABLE `el_" << j << "` (\n"
                "\t`id` mediumint NOT NULL";
        for (int k=0; k<objects.at(i)->properties.size(); k++){
            QString cortege = ",\n\t`%1` %2";
            QString name = objects.at(i)->properties.at(k).first;
            QString type = objects.at(i)->properties.at(k).second;
            
//            if( name == "name")
  //              continue;
            
            //TODO: bool and other types support
            if (type == "string" || type.contains("enum"))
                type = "VARCHAR(100)";
            else if (type == "int")
                type = "INTEGER";
            else if (type == "positiveInt" || type == "nonNegativeInt")
                type = "INTEGER UNSIGNED";
            else if (type == "bool" )    
                type = "BOOL";
            else if (type == "text" )    
                type = "VARCHAR(1000)";
            else 
                type = "INTEGER";
            
            out << cortege.arg(name).arg(type);
           // out << "\n";
        }        
        out << ");\n\n";    
    }

    out << inserts;
    file.close();
    
}

void Generator::genClasses(){
    
    //
    // I. elements
    //
    if( !dir.exists("umllib") )
        dir.mkdir("umllib");
    dir.cd("umllib");
    if( !dir.exists("generated") )
        dir.mkdir("generated");

    QFile file("generated/umllib/generated/objects.cpp");
        if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
            return;
    QTextStream out(&file);

    QFile file2("generated/umllib/generated/objects.h");
        if( !file2.open(QIODevice::WriteOnly | QIODevice::Text) )
            return;
    QTextStream out2(&file2);


    out <<  "#include <QtGui>\n"
            "#include \"objects.h\"\n\n"
            "using namespace UML;\n\n";

    out2 << "#include <QWidget>\n#include <QtSvg/QSvgRenderer>\n#include <QList>\n\n"
            "#include \"uml_nodeelement.h\"\n"
            "#include \"uml_edgeelement.h\"\n"
            "namespace UML {\n";


    for (int i=0; i < objects.size(); i++){

        QString classname = objects.at(i)->id + "Class";

        if( objects.at(i)->type == EDGE )
            continue;

        // 
        // 1. CPP-file
        //
        
        out << "// " << classname << "\n";

        //constructor
        out << classname << "::" << classname << "()\n";
        out <<   "{\n";
	    
        out << "\tupdatePorts();\n"
            << QString("\trenderer.load(QString(\"%1\"));\n").arg(":/shapes/" + classname + ".svg") 
            << "\ttext = \"\";\n";
        if( objects.at(i)->parents.size() > 0)
            out << "\tparentsList";
        for( int j=0;j<objects.at(i)->parents.size(); j++ ){
            out << QString("  << %1").arg(position(objects.at(i)->parents.at(j)) + NUM);
        }    
        out << ";\n";
        out << QString("\theight = %1;\n").arg(objects.at(i)->height)
            << QString("\twidth = %1;\n").arg(objects.at(i)->width)
            << QString("\ttextSize = 0;\n")
            << "}\n\n";

	    //destructor
	    out << classname << "::~" << classname << "()\n";
        out <<   "{\n}\n\n";

        
        //paint
        out << "void " << classname << "::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,"
                                                                                "QWidget *widget)\n{\n";
        out << "\tupdatePorts();\n"
            << QString("\trenderer.render(painter, QRectF(textSize/2-width/2, 0, width, height));\n\n")
            << "\tQTextDocument d;\n\td.setHtml(text);\n"
            << "\tpainter->save();\n"
            << "\tpainter->translate(0,height-5);\n"
            << "\n\td.drawContents(painter,contentsRect());\n"
            << "\tpainter->restore();\n"
            << "\tNodeElement::paint(painter, style, widget);\n"
            << "}\n\n";

        //boundingRect
        out << "QRectF " << classname << "::boundingRect() const\n{\n"
            << QString("\treturn QRectF(-5, -5, textSize+10, height+15);\n")
            << "}\n\n";

        //contentsRect
        out << "QRectF " << classname << "::contentsRect() const\n{\n"
            << QString("\treturn QRectF(0, 0, textSize, height+15);\n")
            << "}\n\n";

        //updateData
        out << "void " << classname << "::updateData()\n{\n"
            << "\tNodeElement::updateData();\n"
            << "\ttext = dataIndex.data().toString();\n"
            << "\tprepareGeometryChange();\n"
            << "\ttextSize = (width > text.size() * 8) ? width : text.size() * 8;\n"
            << "\tupdate();\n"
            << "}\n\n";

        // updatePorts
        out << "void " + classname + "::updatePorts(){\n"
               "\tports.clear();\n";
        if( objects.at(i)->type == NODE){       
            Node* node = (Node*) objects.at(i);
            out << "\tint d = textSize/2 - width/2;\n";
            if( node->ports.size() == 0 ){
                    out <<  "\tports << QPointF(d, height/2) << QPointF(width/2 + d, height)"
                                   " << QPointF(width +d, height/2) << QPointF(width/2+d, 0);\n";
            }          
            for( int j=0; j<node->ports.size(); j++ ){
                if( node->ports.at(j).type == "point" ){
                    out << QString("\tports << QPointF(d+%1, %2);\n").arg(node->ports.at(j).vals.at(0))
                                                          .arg(node->ports.at(j).vals.at(1));
                }                                          
                else if( node->id != "uscnActor" ){
                    out <<  "\tports << QPointF(d, height/2) << QPointF(width/2 + d, height)"
                                   " << QPointF(width +d, height/2) << QPointF(width/2+d, 0);\n";
                }          
            }
        } 
        out << "}\n";
        
        out << "// " + classname  << "\n";

        //
        //2. H-files
        //

        out2 << "#ifndef " << classname.toUpper() << "_H\n#define " << classname.toUpper() << "_H\n\n";

        out2 << "class " << classname << " : public NodeElement{\n";
        out2 << "\tpublic:\n\t\t" << classname << "();\n";
        out2 << "\t\t~" << classname << "();\n";
        out2 <<  "\tvirtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);\n"
                "\tvirtual QRectF boundingRect() const;\n"
                "\tvirtual QRectF contentsRect() const;\n"
                "\tvirtual void updateData();\n\n"
		        "private:\n"
                "\tvoid updatePorts();\n\n"
                "\tQString text;\n"
                "\tint textSize;\n"
                "\tint width;\n"
                "\tint height;\n"
		        "\tQSvgRenderer renderer;\n";
        
        out2 << "};\n\n#endif\n";
    }    
    
    //
    // II. edges
    //

    for ( int i=0; i<edges.size(); i++ ){

        //
        // 1. H-files
        //

        QString classname = edges.at(i)->id + "Class";
        out2 << "#ifndef " << classname.toUpper() << "_H\n#define " << classname.toUpper() << "_H\n\n";

        out2 << "\tclass " << classname << " : public EdgeElement{\n";
        out2 << "\tpublic:\n\t\t" << classname << "();\n";
        out2 << "\t\t~" << classname << "();\n";
        out2 <<  "\t\tvirtual void drawStartArrow(QPainter *) const;\n"
                "\t\tvirtual void drawEndArrow(QPainter *) const;\n"
		        "\tprivate:\n"
		        "\t\tQSvgRenderer renderer;\n";
        out2 << "};\n\n#endif\n";
        
        // 
        // CPP-files
        //

        out << classname << "::" << classname << "()\n";
        out <<   "{\n}\n\n";
        
        out << classname << "::~" << classname << "()\n";
        out <<   "{\n}\n\n";

        out << "void " << classname << "::drawStartArrow(QPainter *) const\n";
        out <<   "{\n}\n\n";
        
        out << "void " << classname << "::drawEndArrow(QPainter *) const\n";
        out <<   "{\n}\n\n";
 
    }
    out2 << "}\n";   
    file.close();
    file2.close();
    dir.cdUp();

    // 
    // IV. pri-file
    //

    QFile file3("generated/umllib/generated/umllib.pri");
    if( !file3.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out3(&file3);
    QString headers = "HEADERS += umllib/generated/objects.h\n";
    QString sources = "SOURCES += umllib/generated/objects.cpp\n";
    /*QString prefix = "umllib/generated/";
    
    for (int i=0; i < objects.size(); i++){
        
        headers += " \\ \n\t" + prefix + objects.at(i)->id + "Class.h";
        sources += " \\ \n\t" + prefix + objects.at(i)->id + "Class.cpp";

    }        
    */
    out3 << headers << "\n\n";
    out3 << sources << "\n";
    file3.close();
}



void Generator::genFactory()
{

    if( !dir.exists("umllib"))
        dir.mkdir("umllib");
    QFile file("generated/umllib/uml_guiobjectfactory.cpp");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
    QString includes = "";
    QString classes = "";
    QString tmp = "\t\tcase UML::%1:\n\t\t\treturn new %2();\n";
    
    out <<  "#include <QtGui>\n\n"
	        "#include \"realreporoles.h\"\n"
            "#include \"objects.h\"\n"
            "#include \"uml_guiobjectfactory.h\"\n\n";


    for (int i=0; i<objects.size(); i++){
        int height = objects.at(i)->height;
        int width  = objects.at(i)->width;

        if ( height == -1 && width == -1 )
            continue;
        classes += tmp.arg(objects.at(i)->id).arg(objects.at(i)->id + "Class") ;
    }

    for (int i=0; i<edges.size(); i++){
    
        classes += tmp.arg(edges.at(i)->id).arg(edges.at(i)->id + "Class");

    }
    
  //  out << includes << "\n\n";
    out <<  "using namespace UML;\n\n"
            "Element * UML::GUIObjectFactory(int type){\n"
            "\tswitch ( type ) {\n";
    out << classes;

    out << "\t\tdefault: \t qDebug() << \"not creating\" << type; \n\t}\n";
    out << "\treturn 0;\n}\n";

    file.close();
}

int Generator::position( QString id ){
    
    int result = -1;
    for( int i=0; i<(int) objects.size(); i++ )
        if( objects.at(i)->id == id ){
            result = i;
            break;
        }
   return result; 
    
}

void Generator::genRealRepoInfo(){

    QFile file("generated/repo/realrepoinfo.h");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);

    out << "#include <QStringList>\n#include <QMap>\n#include <QString>\n\n";

    out <<  "class RealRepoInfo\n{\n"
            "public:\n"
            "\tRealRepoInfo();\n"
            "\t~RealRepoInfo();\n"
            "\tQStringList getObjectCategories() const;\n"
            "\tQList<int> getObjects(int category) const;\n"
            "\tQString objectDesc(int id) const;\n"
            "\tQString objectName(int id) const;\n\n"
            "\tQStringList getColumnNames(int type) const;\n"
            "\tQString getColumnName(int type, int role) const;\n\n"
            "\tint roleByIndex(int index) const\n"
	    "\t\t{ return index+129; };\n"
            "\tint indexByRole(int role) const\n"
	    "\t\t{ return role-129; };\n\n"
            "private:\n"
            "};\n";
   
    file.close();

    QFile file2("generated/repo/realrepoinfo.cpp");
    if( !file2.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out2(&file2);

    // static inits
    out2 << "#include \"realrepoinfo.h\"\n\n"
            "static bool initCompleted = false;\n\n"

	    "class Category\n{\n"
            "public:\n"
            "\tQString name;\n"
            "\tQList<int> objects;\n};\n\n"

            "static QList< Category > categories;\n"
            "static QStringList objects;\n"
            "static QStringList descriptions;\n"
            "static QMap<int, QStringList> map;\n\n";

    out2 << "static void initStaticData()\n{\n"
            "\tCategory cat;\n\n";
    for( int i=0; i<categories.size(); i++){
        out2 << QString("\tcat.objects.clear();\n\tcat.name = \"%1\";\n").arg(categories.at(i)->name);
        if( categories.at(i)->objects.size() > 0 )
            out2 << QString("\tcat.objects ");
        for( int j=0; j<categories.at(i)->objects.size(); j++)
            out2 << QString(" << %1").arg(categories.at(i)->objects.at(j)+NUM);
        out2 << "\t;\n";    
        out2 << "\tcategories << cat;\n\n";
    }
    if( objects.size() > 0 )
        out2 << "objects ";
    for( int i=0; i<objects.size(); i++ )
        out2 << QString(" << \"%1\"").arg(objects.at(i)->id);
    out2 << ";\n\n";    
    if( objects.size() > 0 )
        out2 << "descriptions ";
    for( int i=0; i<objects.size(); i++ ){
        out2 << QString(" << \"%1\"").arg(objects.at(i)->name);
    }

    out2 << ";\n\n";

    // from former realreporoles
    
    out2 << "// from realreporoles.cpp\n\n";
    out2 << "\tQStringList l;\n";

    for (int i=0; i<objects.size(); i++){
        out2 << "\tl.clear();\n";
        for( int j=0; j<objects.at(i)->properties.size(); j++)
            out2 << QString("\t\tl << \"%1\";\n").arg(objects.at(i)->properties.at(j).first);
        out2 << QString("\tmap.insert(%1, l);\n").arg(NUM + i);
        out2 << "\n";
    }

    out2 << "\n}\n\n";

    // constructor

    out2 << "RealRepoInfo::RealRepoInfo()\n"
            "{\n\tif ( ! initCompleted )\n"
            "\t\tinitStaticData();\n"
            "\tinitCompleted = true;\n}\n\n";
                
    // destructor
    out2 << "RealRepoInfo::~RealRepoInfo()\n{\n}\n\n";

    // getObjectCategories
    out2 << "QStringList RealRepoInfo::getObjectCategories() const\n{\n"
            "\tQStringList l;\n"
            "\tfor( int i=0; i<categories.size(); i++)\n"
            "\t\tl << categories.at(i).name;\n"
            "\treturn l;\n}\n\n";

    // getObjects
    out2 << "QList<int> RealRepoInfo::getObjects(int category) const\n{\n"
            "\treturn categories.at(category).objects;\n}\n\n";

    // objectName
    out2 << "QString RealRepoInfo::objectName( int id ) const\n{\n"
            "\treturn objects.at(id-1);\n}\n\n";
    
    out2 << "QString RealRepoInfo::objectDesc( int id ) const\n{\n"
            "\treturn descriptions.at(id-1);\n}\n\n";

    // getColumnName

    out2 << "QString RealRepoInfo::getColumnName(int type, int role) const\n{\n"
            "\treturn map.value(type).at(indexByRole(role));\n}\n\n";

    // getColumnNames

    out2 << "QStringList RealRepoInfo::getColumnNames(int type) const\n{\n"
            "\treturn map.value(type);\n}\n\n";

    file2.close();
}


void Generator::genEdgesFunction(){

    QFile file("generated/repo/edges_stuff.cpp");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
    
    out <<  "#include <QMap>\n#include <QHash>\n#include <QDebug>\n\n"
            "#include \"uml_nodeelement.h\"\n"
            "#include \"objects.h\"\n";
    QString cases = "";
    QString singleCase = "\t\tcase %1: return check(pars, new %2()); break;\n";

    for( int i=0; i<objects.size(); i++)
        if( objects.at(i)->type == NODE){
            cases += singleCase.arg(i+NUM).arg(objects.at(i)->id + "Class");
        }
    out << "\n";       
    out <<  "const int ANY = -1;\n"
            "const int NONE = 0;\n\n";

    out << "using namespace UML;\n";        

    out << "int check(QList<int> pars, NodeElement* el){\n"
           "\tfor( int i=0; i<pars.size(); i++)\n"
           "\t\tif( el->isChildOf(pars.at(i)) ){\n"
           "\t\t\treturn pars.at(i);\t\t\n\t}\n\n"
           "\treturn NONE;\n}\n\n";

    out << "int checkParents(QList<int> pars, int id){\n"
           "\tswitch(id){\n"
           "\t\tcase -1: return -1; break;\n";
    out << cases;                        
    out << "\t\tdefault: return NONE; break;\n\t}\n\n";
    out << "}\n\n";


    out << "bool canBeConnected( int linkID, int from, int to ){\n"
           "\tQMap< int, QHash< int, int > > edges;\n"
           "\tQHash< int, int> hash;\n\n";
    
    for( int i=0; i<(int) objects.size(); i++){
        if( objects.at(i)->type == EDGE ){
            Edge* edge = (Edge*) objects.at(i);
            for( int j=0; j<(int) edge->associations.size(); j++ ){
                out << "\thash.clear();\n";        
                int from = position(edge->associations.at(j)->from);
                int to = position(edge->associations.at(j)->to);
                if( from != -1)
                    from += 12;
                 if( to != -1)
                    to += 12;
                out << QString("\thash.insertMulti(%1, %2);\n").arg(from).arg(to); 
                out << QString("\tedges.insert(%1, hash);\n\n").arg(i+NUM);
            }
        }
    } 
     
    out << "\tif( from == ANY && to == ANY )\n"
           "\t\treturn true; // (-1, -1)\n\n"
           "\tQHash<int, int> h;\n"
           "\tbool res;\n"
           "\tint fromID = NONE;\n"
           "\tint toID = NONE;\n"
           "\th = edges.value(linkID);\n\n"
           "\tif( h.keys().contains(from) )\n"
           "\t\tfromID = from;\n"
           "\telse\n"
           "\t\tfromID = checkParents(h.keys(), from);\n\n"
           "\tif( h.values().contains(to) )\n"
           "\t\ttoID = to;\n"
           "\telse\n"
           "\t\ttoID = checkParents(h.values(), to);\n\n"
           "\tif( fromID == ANY && toID != NONE)\n"
           "\t\treturn h.values().contains(toID);\n\n"
           "\tif( toID == ANY && fromID != NONE)\n"
           "\t\treturn h.keys().contains(fromID);\n\n"
           "\tres = h.values(fromID).contains(toID);\n\n"
           "\treturn res;\n";
                                                                                                                    
    out << "}\n";

    file.close();
}

Entity* Generator::find( QString id ){
    for ( int i=0; i < (int) objects.size(); i++ )
        if( objects.at(i)->id == id )
            return objects.at(i);
    return 0;    
}

Edge* Generator::findEdge( QString id ){
    for ( int i=0; i < (int) edges.size(); i++ )
        if( edges.at(i)->id == id )
            return edges.at(i);
    return 0;    
}

void Generator::propagateAll(){

    // propagating objects' properties
    for( int i=0; i < (int) objects.size(); i++ ){
        if( !objects.at(i)->propsPropagated )
            propagateProperties( objects.at(i) );
    }

    for( int i=0; i< (int) objects.size(); i++ ){
        if( !objects.at(i)->parentsPropagated )
            propagateParents(objects.at(i));
    }

    for( int i=0; i< (int) objects.size(); i++ ){
//        qDebug() << i + NUM << objects.at(i)->id << objects.at(i)->parents;
    }

    // is not needed right now
/*    
    // propagating edges' stuff    
    for( int i=0; i < (int) edges.size(); i++ ){
        if( !edges.at(i)->assocsPropagated )
            propagateAssocs( edges.at(i) );
    }
  */  
    
  
}

void Generator::propagateParents( Entity* cur ){

    for( int j=0; j < cur->parents.size(); j++ ){
        Entity* par = find(cur->parents.at(j));
        if( !par ){
            cur->parentsPropagated = true;
            return;
        }
        cur->addParent(par->id);
        if ( !par->parentsPropagated )
            propagateParents( par );
        for( int i=0; i<par->parents.size(); i++)
            cur->addParent(par->parents.at(i));
    }
    cur->parentsPropagated = true;
   
}

/*
void Generator::findChildren(Entity* cur, QString id){
    for( int i=0; i<(int) cur->parents.size(); i++ ){
        Entity* par = find(cur->parents.at(i));
        par->addChild(cur->id);
        for( int ch=0; ch<cur->children.size(); ch++ )
            par->addChild(cur->children.at(ch));
        findParents(par, id);

    }
}
*/
void Generator::propagateProperties( Entity* cur ){
   
    for( int j=0; j < cur->parents.size(); j++ ){
        Entity* par = find(cur->parents.at(j));
        if( !par){
            cur->propsPropagated = true;
            return;
        }
        if ( !par->propsPropagated )
            propagateProperties( par );
        for( int k=0; k < par->properties.size(); k++ )
            cur->addProperty( par->properties.at(k).first, par->properties.at(k).second );
    }
    cur->propsPropagated = true;
}

void Generator::propagateAssocs( Edge* cur ){

    for( int j=0; j < cur->parents.size(); j++ ){
        Edge* par = findEdge(cur->parents.at(j));
        if( !par ){
            cur->assocsPropagated = true;
            return;
        }    
        if ( !par->assocsPropagated )
            propagateAssocs( par );
        for( int k=0; k < par->associations.size(); k++ )
            cur->addAssociation( par->associations.at(k) );
    }
    cur->assocsPropagated = true;
}
