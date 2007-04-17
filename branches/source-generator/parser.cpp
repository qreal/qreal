#include "parser.h"

Parser::Parser( QStringList l ){
    files = l;
    dir.cd(".");
    dir.mkdir("generated");
    dir.cd("generated");
    for (int i=0; i<l.size(); i++)
        run(l.at(i));
//    display();
    genEnums();
    genSQLScripts();
    genMappings();
    genClasses();
    genFactory();
    qDebug() << "done";
}

Parser::~Parser(){
}

void Parser::run(QString filename){

    QDomDocument *doc = new QDomDocument("new");
    QFile file(filename);
    if( !file.open(QIODevice::ReadOnly)){
        qDebug() << "incorrect filename";
        return;
    }
    if( !doc->setContent(&file)){
        file.close();
        qDebug() << "cannot set qdomdocument's content";
        return;
    }
    file.close();
   
    // I. enums
    QDomNodeList enums = doc->elementsByTagName("enumType"); 

    for( int i=0; i < (int) enums.length(); i++){
        QDomElement cur = enums.at(i).toElement();
        QStringList values;
        QString name = cur.attribute("id");
        QDomNodeList vals = cur.elementsByTagName("enumValue");
        for( int k = 0; k < (int) vals.length(); k++ )
            values << vals.at(k).toElement().text();
        enumerations.insert(name, values);
    }

    //qDebug() << enumerations;


///////////////////////////////////////////////////////////////////////////////////////////////////
   
    // II. nodes
    nodes = doc->elementsByTagName("node"); 

    for( int ii=0; ii< (int)nodes.length(); ii++ ){
        Entity *cur = new Entity();
        cur->id  = nodes.at(ii).toElement().attribute("id");
        QDomElement logic = nodes.at(ii).firstChildElement("logic");
        
        // 1. generalizations
        //TODO: multiple generalizations support
        //TODO: forward declaration support
        QDomElement generalization = logic.firstChildElement("generalizations").firstChildElement("generalization"); // searching for <generalization> tag
        if( generalization != QDomElement() ){
            QString parentID = generalization.firstChildElement("parent").attribute("parent_id");
            Entity* par = find(parentID);
            if( par ){
                for( int i=0; i<par->properties.size(); i++){
                    if (!cur->properties.contains(par->properties.at(i)))
                        cur->properties << par->properties.at(i);
                }
            }        
                // TODO: associations generalization
        }
        
        // 2. properties
        QDomNodeList props = logic.elementsByTagName("properties");
        for( int i=0; i<props.size(); i++){  // for each <properties>...</properties> block
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
                //qDebug() << i << j << property.at(j).hasAttributes() << property.at(j).toElement().hasChildNodes();
                if( type == "enum" ){
                    type = "enum " + property.at(j).firstChildElement("enum").attribute("idref");
                }
                if( type == "ref" ){
                    type = property.at(j).firstChildElement("ref").attribute("idref");
                } 
                if( !name.isEmpty())
                    if( !cur->properties.contains(QPair<QString, QString>(name, type)))
                        cur->properties << QPair<QString, QString>(name, type);
                
                // TODO: defaults and other missing property stuff support               
            } 
        }
        
        

        // TODO: 3. associations

        // 4. SVG stuff
        QDomNodeList svg = nodes.at(ii).toElement().elementsByTagName("svg:svg");
//        qDebug() << svg.text();
        if (!svg.isEmpty()){
            cur->height = svg.at(0).toElement().attribute("height").toInt();
            cur->width = svg.at(0).toElement().attribute("width").toInt();
        }
        else {
            cur->height = -1;
            cur->width = -1;
        }    

        objects << cur;    
    }
    
///////////////////////////////////////////////////////////////////////////////////////////////////
    // III. edges

    QDomNodeList edges = doc->elementsByTagName("edge");
    
    for( int i=0; i< (int)edges.length(); i++ ){
        Entity *cur = new Entity();
        cur->id  = edges.at(i).toElement().attribute("id");
        QDomElement logic = edges.at(i).firstChildElement("logic");
        
        cur->properties << QPair<QString, QString>("from", "string");
        cur->properties << QPair<QString, QString>("to", "string");
        cur->properties << QPair<QString, QString>("fromPort", "string");
        cur->properties << QPair<QString, QString>("toPort", "string");

        // generalizations
        //TODO: multiple generalizations support
        QDomElement generalization = logic.firstChildElement("generalizations").firstChildElement("generalization"); // searching for <generalization> tag
        if( generalization != QDomElement() ){
            QString parentID = generalization.firstChildElement("parent").attribute("parent_id");
            Entity* par = find(parentID);
            if( par ){
                for( int i=0; i<par->properties.size(); i++){
                    if (!cur->properties.contains(par->properties.at(i)))
                        cur->properties << par->properties.at(i);
                }
            }        
        }
        
   
   
        // 2. properties
                
        QDomNodeList props = logic.elementsByTagName("properties");
        for( int i=0; i<props.size(); i++){  // for each <properties>...</properties> block
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
                //qDebug() << i << j << property.at(j).hasAttributes() << property.at(j).toElement().hasChildNodes();
                if( type == "enum" ){
                    type = "enum " + property.at(j).firstChildElement("enum").attribute("idref");
                }
                if( type == "ref" ){
                    type = property.at(j).firstChildElement("ref").attribute("idref");
                } 
                if( !name.isEmpty())
                    cur->properties << QPair<QString, QString>(name, type);
                
                // TODO: defaults and other missing property stuff support               
            } 
        }
 
        cur->height = -1;
        cur->width = -1;
        objects << cur;    
    
    }

}

Entity* Parser::find( QString id )
{
    for( int i=0; i<objects.size(); i++){
        if( objects.at(i)->id == id )
            return objects.at(i);
    } 
    return 0;
}

void Parser::display()
{
    qDebug() << "elements: " << objects.size(); 
    for( int i=0; i<objects.size(); i++ ){
        qDebug() << objects.at(i)->id;
        for (int j=0; j<objects.at(i)->properties.size(); j++)
            qDebug() << objects.at(i)->properties.at(j).first << "\t\t" << objects.at(i)->properties.at(j).second;
        
        qDebug() << "";
    }
}

void Parser::genEnums()
{
    if( !dir.exists("repo") )
        dir.mkdir("repo");
    QFile file("generated/repo/realreporoles.h");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
    
    int id = 12;
    
    out << "#ifndef REALREPOROLES_H\n#define REALREPOROLES_H\n\n";
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
    
    out << "#endif\n";
    
    file.close();
}

void Parser::genSQLScripts()
{
    QFile file("generated/scripts.sql");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
    
    out << "drop database unreal;\n create database unreal;\n use unreal;\n";

    QString inserts = "INSERT INTO `elements_all` (id, name) VALUES ";
    
    for (int i=0; i<objects.size(); i++){
        int j = i+10;
        inserts += QString("\t(%1, '%2'),\n").arg(j).arg(objects.at(i)->id);

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
    
    out << "CREATE TABLE `elements_all` (`id` mediumint NOT NULL, `name` varchar(100) NOT NULL, PRIMARY KEY (id));\n";
    inserts += "\t(666,'fixmeplz');";
    out << inserts;
    file.close();
    
}

void Parser::genMappings()
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

void Parser::genClasses(){
    
    for (int i=0; i < objects.size(); i++){

        int height = objects.at(i)->height;
        int width  = objects.at(i)->width;

        if ( height == -1 && width == -1 )
            continue;

        QString classname = objects.at(i)->id + "Class";
        // 
        // I. cpp-file
        //
        if( !dir.exists("umllib") )
            dir.mkdir("umllib");
        dir.cd("umllib");
        if( !dir.exists("cppcode") )
            dir.mkdir("cppcode");
        
        QFile file("generated/umllib/cppcode/" + classname + ".cpp");
        if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
            return;
        QTextStream out(&file);
    
        out << "#include <QtGui>\n\n";
        out << QString("#include \"" + classname + ".h\"\n\n");
	out << "using namespace UML;\n\n\n";
        
        //constructor
        out << classname << "::" << classname << "()\n";
        out <<   "{\n";
        out << "\tports << " << QString("QPointF(%1, %2)").arg(height/2).arg(0) << " << " <<
                            QString("QPointF(%1, %2)").arg(height/2).arg(width) << " << " <<
                            QString("QPointF(%1, %2)").arg(0).arg(width/2) << " << " <<
                            QString("QPointF(%1, %2)").arg(height).arg(width/2) << ";\n";

	    out << QString("\trenderer.load(QString(\"%1\"));\n").arg(":/shapes/" + classname + ".svg") ;

        out << "}\n\n";

	    //destructor
	    out << classname << "::~" << classname << "()\n\n";
        out <<   "{\n}\n";

        
        //paint
        out << "void " << classname << "::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,"
                                                                                "QWidget *widget)\n{\n";
        out << "\trenderer.render(painter, contentsRect());\n";
        out << "\tNodeElement::paint(painter, style, widget);\n";
        out << "}\n\n";

        //boundingRect
        out << "QRectF " << classname << "::boundingRect() const\n{\n";
        out << QString("\treturn QRectF(%1, %2, %3, %4);\n").arg(-8).arg(-8).arg(height+16).arg(width+16);
        out << "}\n\n";

        //contentsRect
        out << "QRectF " << classname << "::contentsRect() const\n{\n";
        out << QString("\treturn QRectF(%1, %2, %3, %4);\n").arg(0).arg(0).arg(height).arg(width);
        out << "}\n\n";

        file.close();    

        //
        //II. H-file
        //
        QFile file2("generated/umllib/cppcode/" + classname + ".h");
        if( !file2.open(QIODevice::WriteOnly | QIODevice::Text) )
            return;
        QTextStream out2(&file2);

        out2 << "#ifndef " << classname.toUpper() << "_H\n#define " << classname.toUpper() << "_H\n\n";
        out2 << "#include \"uml_nodeelement.h\"\n";
        //out << "#include \"uml_edgeelement.h\"\n";
        out2 << "#include <QWidget>\n#include <QtSvg/QSvgRenderer>\n";

        out2 << "namespace UML {\n";
        out2 << "class " << classname << " : public NodeElement{\n";
        out2 << "\tpublic:\n\t\t" << classname << "();\n";
        out2 << "\t\t~" << classname << "();\n";
        out2 <<  "\tvirtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);\n"
                "\tvirtual QRectF boundingRect() const;\n"
                "\tvirtual QRectF contentsRect() const;\n"
		"private:\n"
		"\tQSvgRenderer renderer;\n";
        out2 << "\t};\n};\n\n#endif\n";
        file2.close();
        dir.cdUp();
    }    

    // 
    // III. pri-file
    //
    if( !dir.exists("build"))
        dir.mkdir("build");
    QFile file("generated/build/umllib.pri");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
    QString headers = "HEADERS += ";
    QString sources = "SOURCES += ";
    QString prefix = "umllib/cppcode/";
    
    for (int i=0; i < objects.size(); i++){

        int height = objects.at(i)->height;
        int width  = objects.at(i)->width;

        if ( height == -1 && width == -1 )
            continue;
        
        headers += " \\ \n\t" + prefix + objects.at(i)->id + "Class.h";
        sources += " \\ \n\t" + prefix + objects.at(i)->id + "Class.cpp";


    }        
    out << headers << "\n\n";
    out << sources << "\n";
    file.close();
}

void Parser::genFactory()
{

    if( !dir.exists("umllib"))
        dir.mkdir("umllib");
    QFile file("generated/umllib/uml_guiobjectfactory.cpp");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;
    QTextStream out(&file);
    QString tmp = "\t\tcase UML::%1:\t\treturn new %2();\n";
    
    out <<  "#include <QtGui>\n\n"
            "#include \"realreporoles.h\"\n\n"
            "#include \"uml_guiobjectfactory.h\"\n\n"
            "#include \"uml_edgeelement.h\"\n"
            "#include \"uml_glamour_class.h\"\n\n"
            "using namespace UML;\n\n"
            "Element * UML::GUIObjectFactory(int type){\n"
            "\tswitch ( type ) {\n";


    for (int i=0; i<objects.size(); i++){
        int height = objects.at(i)->height;
        int width  = objects.at(i)->width;

        if ( height == -1 && width == -1 )
            continue;
        out << tmp.arg(objects.at(i)->id).arg(objects.at(i)->id + "Class");
    }

    out << "\t\tdefault: \t qDebug() << \"not creating\" << type; \n\t}\n";
    out << "\treturn 0;\n}\n";

    file.close();
}

