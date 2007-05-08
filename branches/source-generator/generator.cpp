#include "generator.h"

Generator::Generator( QStringList files ){

    // inits
    resources = "<!DOCTYPE RCC><RCC version=\"1.0\">\n<qresource>\n";
    res = "\t<file>%1</file>\n";

    untitled = 0;

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
    genMappings();
    genClasses();
    genFactory();
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
    parseSVG( cur, dnode ); 

    cur->type = NODE;
    objects << cur;

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
   
    out << "drop database unreal2;\ncreate database unreal2;\nuse unreal2;\n\n";

    out << "CREATE TABLE nametable (\n"
            "\tid MEDIUMINT NOT NULL,\n"
            "\ttype MEDIUMINT NOT NULL,\n"
            "\tname VARCHAR(255),\n"
            "\tqualifiedName VARCHAR(255)\n"
            ");\n\n"
            "CREATE TABLE metatable (\n"
            "\tid MEDIUMINT NOT NULL,\n"
            "\tname VARCHAR(255),\n"
            "\tqualifiedName VARCHAR(255)\n"
            ");\n\n"
            "CREATE TABLE diagram (\n"
            "\tdiagram_id MEDIUMINT NOT NULL,\n"
            "\tel_id MEDIUMINT NOT NULL,\n"
            "\tx MEDIUMINT,\n"
            "\ty MEDIUMINT\n"
            ");\n\n";
    //TODO: remove
        out <<  "CREATE TABLE `max_uuid` ( `id` mediumint  NOT NULL );\n\n"
                "INSERT INTO `max_uuid` VALUES (100);\n\n"
                "DELIMITER ;;\n\n"
                "CREATE FUNCTION get_id() RETURNS mediumint\n"
                "BEGIN\n"
                "\tDECLARE x mediumint;\n"
                "\tUPDATE max_uuid SET id=id+1;\n"
                "\tSELECT id FROM max_uuid INTO x;\n"
                "\tRETURN x;\n"
                "END;\n"
                ";;\n\n"
                "DELIMITER ;\n\n";
                                                                                                                                            
    QString ins = "INSERT INTO `metatable` (id, name, qualifiedName) VALUES (%1, '%2', '%3');\n";
    QString ins2 = "INSERT INTO `el_0` (id, name) VALUES (%1, '%2');\n"; //TODO: remove

    QString inserts = "";
    
    inserts += ins2.arg(10).arg("Diagram");
    
//    inserts += ins.arg(10).arg("Diagram");
    
    for (int i=0; i<objects.size(); i++){
        int j = i+NUM;
        inserts += ins.arg(j).arg(objects.at(i)->id).arg(objects.at(i)->name);
        inserts += ins2.arg(j).arg(objects.at(i)->id);
                

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
    //TODO: remove
    out << "CREATE TABLE `el_0` (`id` mediumint NOT NULL, `name` varchar(100) NOT NULL, PRIMARY KEY (id));\n";
    out << "CREATE TABLE `el_10` (`id` mediumint NOT NULL, `name` varchar(100) NOT NULL, PRIMARY KEY (id));\n";
    //inserts += ";";

    out << inserts;
    file.close();
    
}

void Generator::genMappings()
{
    if( !dir.exists("repo") )
        dir.mkdir("repo");
    QFile file("generated/repo/realreporoles.cpp");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);

    out << "#include <QString>\n#include <QMap>\n#include <QStringList>\n\n";
    out << "QString getColumnName(int elementNum, int roleNum){\n";
    out << "\tQMap<int, QStringList> map;\n";
    out << "\tQStringList l;\n";
    int elementBase = NUM;
    int roleBase = 129;

    for (int i=0; i<objects.size(); i++){
        out << "\tl.clear();\n";
        for( int j=0; j<objects.at(i)->properties.size(); j++)
            out << QString("\t\tl << \"%1\";\n").arg(objects.at(i)->properties.at(j).first);
        out << QString("\tmap.insert(%1, l);\n").arg(elementBase + i);
        out << "\n";
    }

    out << QString("\treturn map.value(elementNum).at(roleNum-%1);\n").arg(roleBase);
    out << "}\n";
    file.close();
}

void Generator::genClasses(){
    
    //
    // I. elements
    //

    for (int i=0; i < objects.size(); i++){

        int height = objects.at(i)->height;
        int width  = objects.at(i)->width;
/*
        if ( height == -1 && width == -1 )
            continue;
*/
        QString classname = objects.at(i)->id + "Class";
        // 
        // 1. CPP-file
        //
        if( !dir.exists("umllib") )
            dir.mkdir("umllib");
        dir.cd("umllib");
        if( !dir.exists("generated") )
            dir.mkdir("generated");
        
        QFile file("generated/umllib/generated/" + classname + ".cpp");
        if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
            return;
        QTextStream out(&file);
    
        out << "#include <QtGui>\n\n";
        out << QString("#include \"" + classname + ".h\"\n\n");
	    out << "using namespace UML;\n\n\n";
       
        //constructor
        out << classname << "::" << classname << "()\n";
        out <<   "{\n";
        out << "\tports << " << QString("QPointF(%1, %2)").arg(2*height/3).arg(0) << " << " <<
                            QString("QPointF(%1, %2)").arg(2*height/3).arg(4*width/3) << " << " <<
                            QString("QPointF(%1, %2)").arg(0).arg(2*width/3) << " << " <<
                            QString("QPointF(%1, %2)").arg(4*height/3).arg(2*width/3) << ";\n";

	    out << QString("\trenderer.load(QString(\"%1\"));\n").arg(":/shapes/" + classname + ".svg") ;
        out << "\ttext = \"\";\n";
        if( objects.at(i)->parents.size() > 0)
            out << "\tparentsList";
        for( int j=0;j<objects.at(i)->parents.size(); j++ ){
            out << QString("  << %1").arg(position(objects.at(i)->parents.at(j)) + NUM);
        }    
        out << ";\n}\n\n";

	    //destructor
	    out << classname << "::~" << classname << "()\n";
        out <<   "{\n}\n\n";

        
        //paint
        out << "void " << classname << "::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,"
                                                                                "QWidget *widget)\n{\n";
        out <<QString("\trenderer.render(painter, QRectF(%1, %2, %3, %4));\n").arg(5).arg(height/2).arg(height).arg(width);
        out << "\n\tQTextDocument d;\n\td.setHtml(text);\n"
                    "\n\td.drawContents(painter,QRectF(0,0,0,0));\n\n";
        out << "\tNodeElement::paint(painter, style, widget);\n";
        out << "}\n\n";

        //boundingRect
        out << "QRectF " << classname << "::boundingRect() const\n{\n";
        out << QString("\treturn QRectF(%1, %2, %3, %4);\n").arg(-height/2).arg(-height/2).arg(2*height).arg(2*height);
        out << "}\n\n";

        //contentsRect
        out << "QRectF " << classname << "::contentsRect() const\n{\n";
        out << QString("\treturn QRectF(%1, %2, %3, %4);\n").arg(-5).arg(-5).arg(height).arg(2*width/3);
        out << "}\n\n";

        //updateData
        out << "void " << classname << "::updateData()\n{\n";
        out << "\tNodeElement::updateData();\n";
        out << "\ttext = dataIndex.data().toString();\n"; 
        out << "\tupdate();\n";
        out << "}\n\n";

        file.close();    

        //
        //2. H-files
        //
        QFile file2("generated/umllib/generated/" + classname + ".h");
        if( !file2.open(QIODevice::WriteOnly | QIODevice::Text) )
            return;
        QTextStream out2(&file2);

        out2 << "#ifndef " << classname.toUpper() << "_H\n#define " << classname.toUpper() << "_H\n\n";
        out2 << "#include \"uml_nodeelement.h\"\n";
        out2 << "#include <QWidget>\n#include <QtSvg/QSvgRenderer>\n#include <QList>\n\n";

        out2 << "namespace UML {\n";
        out2 << "class " << classname << " : public NodeElement{\n";
        out2 << "\tpublic:\n\t\t" << classname << "();\n";
        out2 << "\t\t~" << classname << "();\n";
        out2 <<  "\tvirtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);\n"
                "\tvirtual QRectF boundingRect() const;\n"
                "\tvirtual QRectF contentsRect() const;\n"
                "\tvirtual void updateData();\n\n"
		        "private:\n"
                "\tQString text;\n"
		        "\tQSvgRenderer renderer;\n";
        
        out2 << "\t};\n};\n\n#endif\n";
        file2.close();
        dir.cdUp();
    }    
    
    //
    // II. edges
    //

    for ( int i=0; i<edges.size(); i++ ){

        //
        // 1. H-files
        //

        QString classname = edges.at(i)->id + "Class";
        QFile f("generated/umllib/generated/" + classname + ".h");
        if( !f.open(QIODevice::WriteOnly | QIODevice::Text) )
            return;
        QTextStream out(&f);
        
        out << "#ifndef " << classname.toUpper() << "_H\n#define " << classname.toUpper() << "_H\n\n";
        out << "#include \"uml_edgeelement.h\"\n";
        out << "#include <QWidget>\n#include <QtSvg/QSvgRenderer>\n#include <QList>\n\n";

        out << "namespace UML {\n";
        out << "\tclass " << classname << " : public EdgeElement{\n";
        out << "\tpublic:\n\t\t" << classname << "();\n";
        out << "\t\t~" << classname << "();\n";
        out <<  "\t\tvirtual void drawStartArrow(QPainter *) const;\n"
                "\t\tvirtual void drawEndArrow(QPainter *) const;\n"
		        "\tprivate:\n"
		        "\t\tQSvgRenderer renderer;\n";
        out << "\t};\n};\n\n#endif\n";
        f.close();
        
        // 
        // CPP-files
        //

        QFile f2("generated/umllib/generated/" + classname + ".cpp");
        if( !f2.open(QIODevice::WriteOnly | QIODevice::Text) )
            return;
        QTextStream out2(&f2);
        
        out2 << "#include <QtGui>\n\n";

        out2 << QString("#include \"%1\"\n\n").arg(classname + ".h");

        out2 << "using namespace UML;\n\n";

        out2 << classname << "::" << classname << "()\n";
        out2 <<   "{\n}\n\n";
        
        out2 << classname << "::~" << classname << "()\n";
        out2 <<   "{\n}\n\n";

        out2 << "void " << classname << "::drawStartArrow(QPainter *) const\n";
        out2 <<   "{\n}\n\n";
        
        out2 << "void " << classname << "::drawEndArrow(QPainter *) const\n";
        out2 <<   "{\n}\n\n";
 
        f2.close();
    }
    
    // 
    // IV. pri-file
    //

    QFile file("generated/umllib/generated/umllib.pri");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
    QString headers = "HEADERS += ";
    QString sources = "SOURCES += ";
    QString prefix = "umllib/generated/";
    
    for (int i=0; i < objects.size(); i++){

    //    int height = objects.at(i)->height;
    //    int width  = objects.at(i)->width;

 //       if ( height == -1 && width == -1 )
   //         continue;
        
        headers += " \\ \n\t" + prefix + objects.at(i)->id + "Class.h";
        sources += " \\ \n\t" + prefix + objects.at(i)->id + "Class.cpp";


    }        
/*
    for ( int i=0; i < edges.size(); i++){
    
        headers += " \\ \n\t" + prefix + edges.at(i)->id + "Class.h";
        sources += " \\ \n\t" + prefix + edges.at(i)->id + "Class.cpp";
    }
*/
    out << headers << "\n\n";
    out << sources << "\n";
    file.close();
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
    QString tmp2 = "#include \"%1\"\n";
    
    out <<  "#include <QtGui>\n\n"
	    "#include \"realreporoles.h\"\n"
            "#include \"uml_guiobjectfactory.h\"\n\n";

    for (int i=0; i<objects.size(); i++){
        int height = objects.at(i)->height;
        int width  = objects.at(i)->width;

        if ( height == -1 && width == -1 ){
            continue;
        }    
    	out << QString("#include \"%1Class.h\"\n").arg(objects.at(i)->id);

    }

    for (int i=0; i<edges.size(); i++)
        out << QString("#include \"%1Class.h\"\n").arg(edges.at(i)->id);

    for (int i=0; i<objects.size(); i++){
        int height = objects.at(i)->height;
        int width  = objects.at(i)->width;

        if ( height == -1 && width == -1 )
            continue;
        classes += tmp.arg(objects.at(i)->id).arg(objects.at(i)->id + "Class") ;
        includes += tmp2.arg(objects.at(i)->id + "Class.h");
    }

    for (int i=0; i<edges.size(); i++){
    
        classes += tmp.arg(edges.at(i)->id).arg(edges.at(i)->id + "Class");
        includes += tmp2.arg(edges.at(i)->id + "Class.h");

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

void Generator::genEdgesFunction(){

    QFile file("generated/repo/edges_stuff.cpp");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
    
    out <<  "#include <QMap>\n#include <QHash>\n#include <QDebug>\n\n"
            "#include \"uml_nodeelement.h\"\n";
    QString cases = "";
    QString singleCase = "\t\tcase %1: return check(pars, new %2()); break;\n";

    for( int i=0; i<objects.size(); i++)
        if( objects.at(i)->type == NODE){
            out << "#include \"" + objects.at(i)->id + "Class.h\"\n";
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
           "\t\ttoID = checkParents(h.keys(), to);\n\n"
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
                                                                                                                    
    out << "};\n";

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
        qDebug() << i + NUM << objects.at(i)->id << objects.at(i)->parents;
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
