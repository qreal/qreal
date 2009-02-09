/** @file server.h
 * 	@brief Сервер репозитория
 * */
#pragma once

#include <QTcpServer>
#include "../common/classes.h"
#include "generated/repotypesinfo.h"

namespace reposerver
{
  /** @class QRealRepoServer
   *  @brief Сервер репозитория
   * */
  class QRealRepoServer : public QTcpServer
  {
	Q_OBJECT

  public:
	QRealRepoServer(QObject *const parent = 0);
	~QRealRepoServer();

  public slots:
	/** @brief Отладочная печать */
	void printout() const;

  protected:
	/** @brief Обработать входящее соединение */
	void incomingConnection(int socketDescriptor /**< Дескриптор сокета */);

  private:
	/** @brief Вспомогательная переменная для генерации идентификаторов элементов */
	int mCount;

	/** @brief Объект, хранящий описания типов элементов */
	RepoTypesInfo *mTypesInfo;
	/** @brief Объект, хранящий все элементы репозитория */
	RepoData *mRepoData;
  };

}
