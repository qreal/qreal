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
	 * Returns port ID in terms described below *'Useful information' before class*.
	 * @param location For this point port will be seen. location is assumed to be in LOCAL NodeElement coordinates!
	 * @return Port ID in terms described below *'Useful information' before class*.
	 */
	qreal portId(QPointF const &location) const;

	/**
	 * Calculates port number.
	 * Number of port considering that point ports begin numeration,
	 * and line ports go after them.
	 * @param id qreal Id that will be truncated.
	 * @return Port number.
	 */
	static int portNumber(qreal id);

	/**
	 * Returns port position relative to the top left corner of NodeElement
	 * (position of NodeElement). 
	 * @param id Id that position is returned by this method.
	 * @return Port position relative to the top left corner of NodeElement.
	 */
	QPointF const portPos(qreal id) const;

	/**
	 * Calculates nearest point of NodeElement ports to parameter point. 
	 * @param location To this point nearest port will be calculated. location is assumed to be in SCENE coordinates!
	 * @return Nearest point of NodeElement ports to parameter point.
	 */
	QPointF const nearestPort(QPointF const &location) const;	

	/**
	 * Connects all temporary removed from working NodeElement edges.
	 * Used in model manipulating.
	 */
	void checkConnectionsToPort();

	/**
	 *  Connects edge at working NodeElement position to a port.
	 *  (Calls edge->connectToPort().)
	 */
	void connectLinksToPorts();

	/**
	 * Arranges edges at line ports.
	 */	
	void arrangeLinearPorts();

	/**
	 * Sets GraphicalModelAssistApi.
	 * @param graphicalAssistApi New value for GraphicalModelAssistApi.
	 */
	void setGraphicalAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi);

private:
	/// Value for determing ID of nonexistent port.
	static qreal const mNonexistentPortId = -1; // just smth negative
	
	/**
	 * Maximum fraction part value of line port ID.
	 * They use it for avoiding confusion with next port in our model of port IDs.
	 * Example:
	 * 	N - port Id
	 * 	N + 1 - next port!
	 * 0.9999 is a number that is really close to 1.
	 */
	static qreal const mMaximumFractionPartValue = 0.9999;

	/**
	 * Connects temporary removed links.
	 * Used in model manipulating.
	 * @param temporaryRemovedLinks List of temporary removed links.
	 * @param direction "from" or "to" direction of links.
	 */
	void connectTemporaryRemovedLinksToPort(qReal::IdList const &temporaryRemovedLinks, QString const &direction);

	/**
	 * Returns point port ID that locality contains parameter point. If there is no such locality, it returns mNonexistentPortId.
	 * @param location Point that is considered for locate in locality (kvadratik, kvadratik) of point ports. location is assumed to be in LOCAL NodeElement coordinates!
	 * @return Point port ID that locality contains parameter point. If there is no such locality, it returns mNonexistentPortId.
	 */
	qreal pointPortId(QPointF const &location) const;

	/**
	 * Returns line port point ID that locality contains parameter point. If there is no such locality, it returns mNonexistentPortId.
	 * @param location Point that is considered for locate in locality (kvadratik - 5, kvadratik - 5) of line port points. location is assumed to be in LOCAL NodeElement coordinates!
	 * @return line port point ID that locality contains parameter point. If there is no such locality, it returns mNonexistentPortId.
	 */
	qreal linePortId(QPointF const &location) const;

	/**
	 * Returns distance from location to closest point port of NodeElement and this port number in list of point ports.
	 * @param location Result will be calculated for this point. location is assumed to be in LOCAL NodeElement coordinates!
	 * @return The closest point port number in list of line ports and distance from location to it.
	 */
	QPair<int, qreal> nearestPointPortNumberAndDistance(QPointF const &location) const;
	
	/**
	 * Returns distance from location to closest line port of NodeElement and this port number in list of line ports.
	 * @param location Result will be calculated for this point. location is assumed to be in LOCAL NodeElement coordinates!
	 * @return The closest line port number in list of line ports and distance from location to it.
	 */
	QPair<int, qreal> nearestLinePortNumberAndDistance(QPointF const &location) const;

	/**
	 * Returns nearest point parameter at line port to point.
	 * @param linePortNumber Number of line port at line port list.
	 * @param location To this point nearest point parameter will be calculated. location is assumed to be in LOCAL NodeElement coordinates!
	 * @return Nearest point parameter at line port to point.
	 */
	qreal nearestPointOfLinePort(int const linePortNumber, QPointF const &location) const;

	/**
	 * Returns minimum distance from line port to point.
	 * @param linePortNumber Number of line port at line port list.
	 * @param location To this point distance will be calculated. location is assumed to be in LOCAL NodeElement coordinates!
	 * @return Minimum distance from line port to point.
	 */
	qreal minDistanceFromLinePort(int const linePortNumber, QPointF const &location) const;

	/**
	 * Returns distance between point port and point.
	 * @param pointPortNumber Number of point port at point port list.
	 * @param location To this point distance will be calculated. location is assumed to be in LOCAL NodeElement coordinates!
	 * @return Distance between point port and point.
	 */
	qreal distanceFromPointPort(int const pointPortNumber, QPointF const &location) const;

	/**
	 * Transforms point port for current node size.
	 * @param port Port that will be actually dealt with.
	 */
	QLineF transformPortForNodeSize(StatLine const &port) const;

	/**
	 * Transforms line port for current node size.
	 * @param port Port that will be actually dealt with.
	 */
	QPointF transformPortForNodeSize(StatPoint const &port) const;

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
