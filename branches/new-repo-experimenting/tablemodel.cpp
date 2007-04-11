#include <QtGui>
#include <QtSql>

#include "qsqlconnectiondialog.h"

#include "repo/realrepomodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

  QSqlConnectionDialog dialog(0);
  if (dialog.exec() != QDialog::Accepted)
      return 0;

   QSqlError err;
   QSqlDatabase db = QSqlDatabase::addDatabase(dialog.driverName());
   db.setDatabaseName(dialog.databaseName());
   db.setHostName(dialog.hostName());
   db.setPort(dialog.port());
   if (!db.open(dialog.userName(), dialog.password())) {
       err = db.lastError();
       db = QSqlDatabase();
   }

    if (err.type() != QSqlError::NoError)
	QMessageBox::warning(0, QObject::tr("Unable to open database"), QObject::tr("An error occured while "
                                       "opening the connection:\n") + err.driverText() + "\n" + err.databaseText());

    RealRepoModel model;
//    model.setQuery("select name, id, table_name from elements_all;");

    QMessageBox::information(0, QObject::tr("Dedication"), QObject::tr("This testing program is dedicated to Carlos Castaneda"));					       
					       

    QTreeView *view = new QTreeView;
    view->setModel(&model);
    view->show();

    return app.exec();
}
