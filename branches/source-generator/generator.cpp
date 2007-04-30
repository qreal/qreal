#include "generator.h"

Generator::Generator( QStringList files ){

    // inits
    resources = "<!DOCTYPE RCC><RCC version=\"1.0\">\n<qresource>";
    res = "\t<file>%1</file>\n";

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

    // write the resource file
    QFile file("generated/qtreal.qrc");
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
    QString name = cur.attribute("id");
    QDomNodeList vals = cur.elementsByTagName("enumValue");
    for( int k = 0; k < (int) vals.length(); k++ )
        values << vals.at(k).toElement().text();
    enumerations.insert(name, values);

}

void Generator::parseNode( QDomNode dnode ){

    Node *cur = new Node();
    cur->id  = dnode.toElement().attribute("id");
    QDomElement logic = dnode.firstChildElement("logic");
    
    parseGeneralizations( cur, logic );
    parseProperties( cur, logic );
    // TODO: uncomment
    // parseAssociations( cur, logic );
    parseSVG( cur, dnode ); 

    cur->type = NODE;
    objects << cur;

}

void Generator::parseEdge( QDomNode dnode ){

    Edge *cur = new Edge();
    cur->id  = dnode.toElement().attribute("id");
    QDomElement logic = dnode.firstChildElement("logic");
    
    // quick hack to make these props be on top
    cur->properties << QPair<QString, QString>("from", "string");
    cur->properties << QPair<QString, QString>("to", "string");
    cur->properties << QPair<QString, QString>("fromPort", "string");
    cur->properties << QPair<QString, QString>("toPort", "string");

    parseGeneralizations( cur, logic ); 
    parseProperties( cur, logic );
    parseAssociations( cur, logic );

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

void Generator::parseAssociations( Entity *cur, QDomNode logic ){

    Edge* edge = (Edge*) cur;
    QDomNodeList assocs = logic.toElement().elementsByTagName("association");
    for( int i=0; i < assocs.size(); i++){  
        QString role = assocs.at(i).toElement().attribute("id");
        QDomElement begin = assocs.at(i).firstChildElement("begin");
        QDomElement end = assocs.at(i).firstChildElement("end");
        Association* ass = new Association();
        ass->id = role;
        if( begin != QDomElement() ){
            ass->side  = BEGIN;
            ass->idref = begin.toElement().attribute("idref");
        }    
        if( end != QDomElement() ){
            ass->side = END; 
            ass->idref = end.toElement().attribute("idref");
        }   
        edge->associations << ass;
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
    
    int id = 12;
    
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

    out << "namespace SQLFields {\n\tstatic int ElementOffset = 11;\n\n\tstatic const char * Diagram[] = {\n"
           "\t\t\"x\", \"y\", \"cfg\"\n\t};\n\n\tstatic const char * Package[] = {\n\t\t\"foo!\"\t};\n\n"
           "\tstatic const char * Class[] = {\n\t\t\"properties\", \"methods\"\n\t};\n\n"
           "\tstatic const char * Link[] = {\n\t\t\"from\", \"to\"\n\t};\n\n"
           "\tstatic const char ** ColumnNames[] = {\n\t\tDiagram,\n\t\tPackage,\n\t\tClass,\n\t\tLink\n\t};\n};\n\n";
    
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
   
    out << "drop database unreal2;\n create database unreal2;\n use unreal2;\n";

    QString ins = "INSERT INTO `el_0` (id, name) VALUES (%1, '%2');\n";
    QString inserts = "";
    
    inserts += ins.arg(10).arg("Diagram");
    
    for (int i=0; i<objects.size(); i++){
        int j = i+12;
        inserts += ins.arg(j).arg(objects.at(i)->id);

        out <<  "CREATE TABLE `el_" << j << "` (\n"
                "\t`id` mediumint NOT NULL";
                //"\t'name' VARCHAR2(100) NOT NULL\n";
        for (int k=0; k<objects.at(i)->properties.size(); k++){
            QString cortege = ",\n\t`%1` %2";
            QString name = objects.at(i)->properties.at(k).first;
            QString type = objects.at(i)->properties.at(k).second;

            //TODO: bool and other types support
            if (type == "string" || type.contains("enum"))
                type = "VARCHAR(100)";
            else 
                type = "VARCHAR(1000)";
            
            out << cortege.arg(name).arg(type);
           // out << "\n";
        }        
        out << ");\n\n";    
    }
    
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
    int elementBase = 12;
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

        if ( height == -1 && width == -1 )
            continue;

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
        out << "\ttext = \"\";";
        out << "}\n\n";

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
        out2 << "#include <QWidget>\n#include <QtSvg/QSvgRenderer>\n";

        out2 << "namespace UML {\n";
        out2 << "class " << classname << " : public NodeElement{\n";
        out2 << "\tpublic:\n\t\t" << classname << "();\n";
        out2 << "\t\t~" << classname << "();\n";
        out2 <<  "\tvirtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);\n"
                "\tvirtual QRectF boundingRect() const;\n"
                "\tvirtual QRectF contentsRect() const;\n"
                "\tvirtual void updateData();\n"
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
        out << "#include <QWidget>\n#include <QtSvg/QSvgRenderer>\n";

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

        int height = objects.at(i)->height;
        int width  = objects.at(i)->width;

        if ( height == -1 && width == -1 )
            continue;
        
        headers += " \\ \n\t" + prefix + objects.at(i)->id + "Class.h";
        sources += " \\ \n\t" + prefix + objects.at(i)->id + "Class.cpp";


    }        

    for ( int i=0; i < edges.size(); i++){
    
        headers += " \\ \n\t" + prefix + edges.at(i)->id + "Class.h";
        sources += " \\ \n\t" + prefix + edges.at(i)->id + "Class.cpp";
    }

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

        if ( height == -1 && width == -1 )
            continue;
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
    
    // propagating edges' stuff    
    for( int i=0; i < (int) edges.size(); i++ ){
        if( !edges.at(i)->assocsPropagated )
            propagateAssocs( edges.at(i) );
    }
    
    //TODO: propagating connected nodes' ids
    
  
}

void Generator::propagateProperties( Entity* cur ){
   
    for( int j=0; j < cur->parents.size(); j++ ){
        Entity* par = find(cur->parents.at(j));
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
