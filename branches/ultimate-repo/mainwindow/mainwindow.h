/** @file mainwindow.h
 * 	@brief Главное окно приложения
 * */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QSplashScreen>

#include "ui_mainwindow.h"

#include "connectiondialog.h"

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

	/** @brief Интерфейс главного окна */
	Ui::MainWindowUi ui;

public slots:
	/** @brief Осуществить соединение с репозиторием */
	void connectRepo(QSplashScreen *splash = NULL, const QString &addr = "127.0.0.1", const int port = 6666);
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
	void jumpToAvatarFromScene();
	void jumpToAvatar();

	void toggleShowSplash(bool);

	/** @brief Активировать элемент */
	void activateItemOrDiagram(const QModelIndex &index /**< Индекс выделенного элемента */);

	/** @brief Уничтожает содержимое репозитория и делает реконнект */
	// FIXME: Функцию надо назвать более пристойно, просто лучше я ничего не смог придумать :-(
	void exterminatus(void);

	/** @brief Показать справку */
	void showAbout();
	/** @brief Показать помощь */
	void showHelp();

	/** @brief Обработать нажатие на кнопку печати */
	void print();
	/** @brief Экспортировать диаграмму в формате SVG */
	void makeSvg();

	/** @brief Показывает диалог с опциями. */
	void showOptions();

	/** @brief Реагирует на изменение выделения на сцене, синхронизируя его с диаграм
	 * эксплорером. */
	void sceneSelectionChanged();

	void exportToXmi();

protected:
	virtual void closeEvent(QCloseEvent *event);

private slots:
	/** @brief Исполнить тестовые запросы к репозиторию */
	void runTestQueries() const;
	void reconnect();
	void reconnectRepo(const QString& addr, const int port);
	void generator_editor();

private:
	/** @brief Модель */
	RealRepoModel *model;
	/** @brief Модель редактора свойств */
	PropertyEditorModel propertyModel;

	/** @brief Делегат */
	PropertyEditorDelegate delegate;

	int repoPort;
	QString repoAddress;
	qReal::ConnectionDialog *connDialog;
};

#endif
