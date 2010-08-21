#include "scilabGenerator.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <QtCore/QDebug>

#include "../../kernel/roles.h"
#include "../../../qrrepo/repoApi.h"
#include "../../../utils/outFile.h"

using namespace qReal;
using namespace generators;
using utils::OutFile;

ScilabGenerator::ScilabGenerator(qrRepo::RepoApi const &api, qReal::EditorManager const &editor)
    : mApi(api), mEditorManager(editor)
{
}

void ScilabGenerator::Visit(Id const &id, QTextStream &out)
{
    if (id.element() == "IntegralNode")
        VisitIntegralNode(id, out);
}

gui::ErrorReporter ScilabGenerator::generate(QString const &dirName)
{

    Id repoId = ROOT_ID;
    QString const scilab2cDir = QProcessEnvironment::systemEnvironment().value("SCI2C", ".");
    QString const pathToFile = dirName + "\\test_gen.sci";

    QFile file(pathToFile);
    file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);

    QTextStream out(&file);

    out << "function test_gen()\n";
    IdList allDiagramms = mApi.children(repoId);
    IdList mathDiagramms;

    foreach(Id const aDiagramm,allDiagramms)
    {        
        if (aDiagramm.element() == "MathDiagramNode1")
            mathDiagramms.append(aDiagramm);
    }

    foreach (Id const mathDiagramm, mathDiagramms)
    {
        foreach(Id const child, mApi.children(mathDiagramm))
        {
           Visit(child, out);
        }
    }
    out << "endfunction";
    file.close();
    generateNativeCode(dirName);
    return mErrorReporter;
}

void ScilabGenerator::VisitIntegralNode(const Id &id, QTextStream &out)
{
    QString up_limit_string;
    QString low_limit_string = "";
    QString variable_string = "";
    QString function_string = "";

    up_limit_string = getPropertyString(id, "up limit");
    low_limit_string = getPropertyString(id, "low limit");
    function_string = getPropertyString(id, "function");
    variable_string = getPropertyString(id, "variable");

    out << "x = " << low_limit_string << ":" << up_limit_string << ";\n";
    out << "y = " << function_string << ";\n";
    out << "ans = inttrap(x,y);\n";
    out << "disp(ans)\n";

    out.flush();
}

Id ScilabGenerator::getLinkByPortName(Id const &id, QString const portName)
{
    QStringList list = mEditorManager.getPortNames(id);
    int index = mEditorManager.getPortNames(id).indexOf(portName);
    return mApi.linksByPort(id, index);
}

QString ScilabGenerator::getLinkOtherEntityValue(Id const &link, Id const &id)
{
    QString result;
    if (link.element() != "")
    {
        Id otherEntity = mApi.otherEntityFromLink(link, id);
        if (otherEntity.element() == "ConstantValueNode" && mApi.hasProperty(otherEntity, "value"))
            result =  mApi.property(otherEntity, "value").toString();
        if (otherEntity.element() == "FunctionNode" &&  mApi.hasProperty(otherEntity, "function"))
            result =  mApi.property(otherEntity, "function").toString();

    }

    return result;
}

QString ScilabGenerator::getPropertyString(Id const &id, QString const &propertyName)
{
    QString result = "";
    if (mApi.hasProperty(id, propertyName))
    {
        result = mApi.property(id, propertyName).toString();
        if (result == "")
        {
            Id link = getLinkByPortName(id, propertyName);
            result = getLinkOtherEntityValue(link, id);
            if (result == "")
            {
                result = mEditorManager.getDefaultPropertyValue(id, propertyName);
            }
        }
    }
    return result;
}

void ScilabGenerator::generateNativeCode(QString const &dirName)
{
    QString const scilab2cDir = QProcessEnvironment::systemEnvironment().value("SCI2C", ".");
    QString job1 = "cd " + scilab2cDir;
    QString job2 = "scilab2c('" +  dirName + "\\test_gen.sci', '" + "D:\\QT\\GeneratedFromReal"  +"')";

    #ifdef _MSC_VER
    StartScilab(NULL,NULL,NULL) == FALSE;
    #else
    StartScilab(getenv("SCI") ,NULL,NULL) == FALSE;
    #endif

    SendScilabJob(job1.toAscii().data());
    SendScilabJob("exec loader.sce");
    SendScilabJob(job2.toAscii().data());
}
