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
 * 	In this class port ID represents by qreal type.
 * 	Integral part of ID means number of port considering that point ports begin numeration,
 * 	and line ports go after them.
 * 	In case of line port fractional part ID means position at line port.
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
	 * Returns port position relative to the top left corner of NodeElement
	 * (position of NodeElement). 
	 * @param id Id that position is returned by this method.
	 * @return Port position relative to the top left corner of NodeElement.
	 */
	QPointF const getPortPos(qreal id) const;

	/**
	 * Calculates port number.
	 * Number of port considering that point ports begin numeration,
	 * and line ports go after them.
	 * @param id qreal Id that will be truncated.
	 * @return Port number.
	 */
	static int portNumber(qreal id);

	/**
	 * Calculates nearest point of NodeElement ports to parameter point. 
	 * @param location To this point nearest port is calculated.
	 * @return Nearest point of NodeElement ports to parameter point.
	 */
	QPointF const getNearestPort(QPointF const &location) const;

	/**
	 * Returns port ID in terms described below *'Useful information' before class* 
	 * @return Port ID in terms described below *'Useful information' before class*.
	 */
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

	/**
	 * Sets GraphicalModelAssistApi.
	 * @param graphicalAssistApi New value for GraphicalModelAssistApi.
	 */
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
