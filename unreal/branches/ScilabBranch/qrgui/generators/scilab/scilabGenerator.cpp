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

ScilabGenerator::ScilabGenerator(qrRepo::RepoApi const &api)
    : mApi(api)
{
}

void ScilabGenerator::Visit(Id const &id)
{
    utils::OutFile out("test.sci");

    if (id.element() == "IntegralNode")
        VisitIntegralNode(id, out);
}

gui::ErrorReporter ScilabGenerator::generate()
{
        Id repoId = ROOT_ID;
       // QString outputDirectory = "";//mApi.stringProperty(repoId, "output directory");
//    if (outputDirectory == "")
            //outputDirectory = ".";
   // utils::OutFile out("test.sci");

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
           Visit(child);
        }
    }
}

void ScilabGenerator::VisitIntegralNode(const Id &id, utils::OutFile &out)
{
    QString up_limit_string = "";
    //double d = mApi.portOrderNumberByName(id, "up_limit");
    double d = 0.0;
    Id link = mApi.linksByPort(id, d);

    Id constvalue = mApi.otherEntityFromLink(link, id);
    if (constvalue.element() == "ConstantValueNode")
        up_limit_string = mApi.property(constvalue, "value").toString();

    if (up_limit_string == "")
        up_limit_string = "1";

    out() << "x=a:b;\n";
    out() << "y=sqrt(2*x-1)\n";
    out() << "ans = inttrap(x,y)";
}
