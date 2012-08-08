/** @file borderChecker.h
 * 	@brief Class for checking is argument point belongs to one of NodeElement borders.
 * 	Operates with instant NodeElement state.
 * */

#pragma once

#include <QtCore/QPointF>
#include <QList>

class NodeElement;

class BorderChecker
{
public:
	/**
	 * Constructs a BorderChecker.
	 * @param node Node that is actually dealt with.
	 */
	BorderChecker(const NodeElement* const node);

	/*
	 * TODO: Actually comments have some sort of lie.
	 * There is idea to remove all these stuff from project.
	 */
	/**
	 * Checks is point belongs to NodeElement lower border.
	 * @param point Point that is checked to belong to lower border. 
	 */
	bool checkLowerBorder(QPointF const &point) const;
	/**
	 * Checks is point belongs to NodeElement upper border.
	 * @param point Point that is checked to belong to upper border. 
	 */
	bool checkUpperBorder(QPointF const &point) const;
	/**
	 * Checks is point belongs to NodeElement left border.
	 * @param point Point that is checked to belong to upper border. 
	 */
	bool checkLeftBorder(QPointF const &point) const;
	/**
	 * Checks is point belongs to NodeElement right border.
	 * @param point Point that is checked to belong to right border.
	 */
	bool checkRightBorder(QPointF const &point) const;

	bool checkNoBorderX(QPointF const &point) const; // TODO: rename
	bool checkNoBorderY(QPointF const &point) const;

private:
	/// Node that is actually dealt with.
	const NodeElement* const mNode;

	/**
	 * List of NodeElement border corner coordinates.
	 * Uses only for initializing mXHor, mYHor, mXVert, mYVert constants.
	 */
	QList<double> const mBorderValues;

	/**
	 * Some kind of tabulation for ports.
	 * TODO: understand that it actually is.
	 */
	double const mXHor;
	double const mYHor;
	double const mXVert;
	double const mYVert;
};
