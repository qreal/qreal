#include <QtCore/QFile>

// qrxc -h header_out.h -o source_out.cpp infile.xml

int main (int argc, char *argv[])
{
    if ( argc != 6 ) {
	return 1;
    }

    QFile input(argv[5]);
    if (!input.open(QFile::ReadOnly))
         return 1;

    QByteArray data = input.readAll();
    QString str = QString::fromLocal8Bit(data);


    QFile output(argv[4]);
    if (!output.open(QFile::ReadWrite))
	return 1;

    QFile tmpl_c("plugin_template.cpp");
    if (!tmpl_c.open(QFile::ReadOnly))
         return 1;

    QString tmpl = QString::fromLocal8Bit(tmpl_c.readAll());

    tmpl.replace(QString("@@NAME@@"), str.trimmed());
    tmpl.replace(QString("@@HEADER@@"), argv[2]);
    
    output.write(tmpl.toLocal8Bit());


    QFile header_output(argv[2]);
    if (!header_output.open(QFile::ReadWrite))
        return 1;

    QFile tmpl_h("plugin_template.h");
    if (!tmpl_h.open(QFile::ReadOnly))
         return 1;

    QString headertmpl = QString::fromLocal8Bit(tmpl_h.readAll());

    headertmpl.replace(QString("@@NAME@@"), str.trimmed());
    headertmpl.replace(QString("@@HEADER@@"), argv[2]);


    header_output.write(headertmpl.toLocal8Bit());


    return 0;
}
