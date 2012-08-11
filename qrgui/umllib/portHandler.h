/** @file portHandler.h
 * 	@brief Class for handling ports of NodeElement.
 * 	Provides methods for getting port information, calculating distance between point and port,
 * 	connecting ports and links (some cases).
 * */

#pragma once

#include "../models/graphicalModelAssistApi.h"
#include <QPointF>

/**
 * Useful information:
 * 	In this class port ID represents by 2 types: int, qreal.
 * 	int ID means number of port considering that point ports begin numeration,
 * 	and line ports go after them.
 * 	qreal ID means also number of port (integral part of ID), and in case of line port
 * 	ID means position at line port (fractional part of ID).
 */

class NodeElement;

class PortHandler {
public:
	/**
	 * Constructs a PortHandler.
	 * @param node Node that ports are actually dealt with.
	 * @param graphicalAssistApi GraphicalModelAssistApi that used by node.
	 * @param pointPorts Point ports those are belonged to node.
	 * @param linePorts Line ports those are belonged to node.
	 */
	PortHandler(
			NodeElement *node, qReal::models::GraphicalModelAssistApi *graphicalAssistApi,
			QList<StatPoint> const &pointPorts, QList<StatLine> const &linePorts
			);

	/**
	 * Returns position of 
	 * @param id Id that position is returned by this method.
	 * @return
	 */
	QPointF const getPortPos(qreal id) const;

	/**
	 * Calculates port number. Just floor of id.
	 * @param id qreal Id that will be truncated.
	 * @return Port id in int.
	 */
	static int portId(qreal id);

	QPointF const getNearestPort(QPointF const &location) const;

	qreal getPortId(QPointF const &location) const;

	qreal minDistanceFromLinePort(int const linePortNumber, QPointF const &location) const;
	qreal distanceFromPointPort(int const pointPortNumber, QPointF const &location) const;
	qreal getNearestPointOfLinePort(int const linePortNumber, QPointF const &location) const;
	
	void connectTemporaryRemovedLinksToPort(qReal::IdList const &rtemporaryRemovedLinks, QString const &direction);

	void checkConnectionsToPort();
	void connectLinksToPorts();
	
	void arrangeLinearPorts();

	QLineF newTransform(StatLine const &port) const;
	QPointF newTransform(StatPoint const &port) const;

	void setGraphicalAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi);

private:
	/// Node that ports are actually dealt with.
	NodeElement *mNode;

	/**
	 * GraphicalModelAssistApi that used to reconnect some links
	 * and ports (temporary disconnected and etc).
	 */
	qReal::models::GraphicalModelAssistApi *mGraphicalAssistApi;

	/// List of point ports that belons to mNode.
	QList<StatPoint> const mPointPorts;

	/// List of line ports that belons to mNode.
	QList<StatLine> const mLinePorts;
};
