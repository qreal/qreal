/** @file mainwindow.h
 * 	@brief Главное окно приложения 
 * */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QSplashScreen>

#include "ui_mainwindow.h"

#include "propertyeditorproxymodel.h"
#include "propertyeditordelegate.h"

class RealRepoModel;
class EditorView;

/** @class MainWindow
 * 	@brief Главное окно приложения
 * */
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	MainWindow();
	~MainWindow();

public slots:
	/** @brief Осуществить соединение с репозиторием */
	void connectRepo(QSplashScreen *splash = NULL);
	/** @brief Закрыть репозиторий */
	void closeRepo();
	/** @brief Настроить minimap */
	void adjustMinimapZoom(int zoom /**< Значение масштабирования */);

	/** @brief Осуществить удаление элемента из испектора объектов */
	void deleteFromExplorer();
	/** @brief Осуществить удаление элемента со сцены */
	void deleteFromScene();
	/** @brief Осуществить удаление элемента из инспектора диаграмм */
	void deleteFromDiagram();

	/** @brief Активировать элемент */
	void activateItemOrDiagram(const QModelIndex &index /**< Индекс выделенного элемента */);

	/** @brief Показать справку */
	void showAbout();
	/** @brief Показать помощь */
	void showHelp();

	/** @brief Обработать нажатие на кнопку печати */
	void print();
	/** @brief Экспортировать диаграмму в формате SVG */
	void makeSvg();

	/** @brief I have no idea how to set an encoding in Qt Creator. Function shows Options modal dialog. */
	void showOptions();

protected:
	virtual void closeEvent(QCloseEvent *event);

private:
	/** @brief Модель */
	RealRepoModel *model;
	/** @brief Модель редактора свойств */
	PropertyEditorModel propertyModel;
	
	/** @brief Интерфейс главного окна */
	Ui::MainWindowUi ui;
	
	/** @brief Делегат */
	PropertyEditorDelegate delegate;
};

#endif
