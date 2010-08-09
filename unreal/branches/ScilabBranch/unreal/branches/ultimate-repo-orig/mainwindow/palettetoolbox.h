/** @file palettetoolbox.h
 * 	@brief Класс палитры элементов
 * */
#pragma once

#include <QVBoxLayout>
#include <QComboBox>
#include <QScrollArea>
#include "../common/realrepoapiclasses.h"

class QDragEnterEvent;
class QDropEvent;

/** @class PaletteToolbox
 * 	@brief Класс палитры элементов
 * */
class PaletteToolbox : public QWidget
{
Q_OBJECT

public:
	explicit PaletteToolbox(QWidget *parent = 0);
	~PaletteToolbox();

	/** @brief Установить список видимых редакторов */
	void setEditors(QVector<bool> const &editors);
	/** @brief Получить список видимых редакторов */
	QVector<bool> getSelectedTabs() const;

public slots:
	/** @brief Сделать данный редактор активным */
	void setActiveEditor(int);

protected:
	/** @brief Обработать событие начала операции drag'n'drop */
	void dragEnterEvent(QDragEnterEvent *event /**< Событие */);
	/** @brief Обработать событие завершения операции drag'n'drop*/
	void dropEvent(QDropEvent *event /**< Событие */);
	/** @brief Обработать событие нажатия кнопки мыши*/
	void mousePressEvent(QMouseEvent *event /**< Событие*/);

private:
	/** @class DraggableElement
	 * 	@brief Элемент палитры объектов */
	class DraggableElement : public QWidget {
		public:
			DraggableElement(TypeIdType const &classid, QWidget *parent = NULL);
			/** @brief Получить икноку элемента
			 * 	@brief @return Иконка элемента
			 * 	*/
			QIcon icon()
			{
				return m_icon;
			}
			/** @brief Получить надпись элемента
			 *	@brief @return Надпись элемента
			 * */
			QString text()
			{
				return m_text;
			}
			/** @brief Получить идентификатор элемента
			 *	@brief @return Идентификатор элемента
			 * */
			TypeIdType id()
			{
				return m_id;
			}
		private:
			/** @brief Идентификатор элемента */
			TypeIdType m_id;
			/** @brief Иконка элемента */
			QIcon m_icon;
			/** @brief Надпись элемента */
			QString m_text;
	};

	/** @brief Массив содержимого редакторов */
	QVector<QWidget*> mTabs;
	/** @brief Массив имен редакторов */
	QVector<QString> mTabNames;
	/** @brief Массив свойства видимости редакторов */
	QVector<bool> mShownTabs;
	/** @brief Основной layout */
	QVBoxLayout *mLayout;
	/** @brief Выпадающий список видимых редаторов */
	QComboBox *mComboBox;
	/** @brief Содержимое активного редактора */
	QScrollArea *mScrollArea;
	/** @brief Прямое и обратное отображения между элементами выпадающего списка и списка всех редакторов */
	QVector<int> mComboToRepo, mRepoToCombo;
};
