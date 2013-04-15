#ifndef NODE_H
#define NODE_H

#include "stdafx.h"
#include <list>
#include <math.h>

using namespace std;
using namespace cv;

class Node {
public:
	Node(){	}
	
	Node(cv::Point p) {
		thisPoint = p;
	}

	Node(int x, int y){
		thisPoint = cv::Point(x, y);
	}

	int getX() {	return thisPoint.x;	}
	int getY() {	return thisPoint.y;	}

	cv::Point getPoint() {	return thisPoint;	}

	double getDist(int x, int y) {
		return getDist(cv::Point(x, y));
	}

	double getDist(cv::Point p2) {
		double xDist = thisPoint.x - p2.x;
		double yDist = thisPoint.y - p2.y;
		return (sqrt((double)(xDist*xDist+yDist*yDist)));
	}

	list<cv::Point> getPrevConn() {
		return conn_prev;
	}

	list<cv::Point> getNextConn() {
		return conn_next;
	}

	void addPrevConn(cv::Point toAdd) {
		if (!isPrevConn(toAdd)) conn_prev.push_back(toAdd);
	}

	void addNextConn(cv::Point toAdd) {
		if (!isNextConn(toAdd)) conn_next.push_back(toAdd);
	}

	void deletePrevConn(cv::Point toDelete) {
		if (isPrevConn(toDelete)) conn_prev.remove(toDelete);
	}

	void deleteNextConn(cv::Point toDelete) {
		if (isNextConn(toDelete)) conn_next.remove(toDelete);
	}

	bool isPrevConn(cv::Point toEval) {
		bool found = false;
		list<cv::Point>::const_iterator prev_it;

		for (prev_it = conn_prev.begin(); prev_it != conn_prev.end() && !found; ++prev_it) {
			if (*prev_it == toEval) found = true;
		}
		return found;
	}

	bool isNextConn(cv::Point toEval) {
		bool found = false;
		list<cv::Point>::const_iterator next_it;

		for (next_it = conn_next.begin(); next_it != conn_next.end() && !found; ++next_it) {
			if (*next_it == toEval) found = true;
		}
		return found;
	}
	/*
	friend bool operator==(const Node& lhs, const Node& rhs) {
		return (lhs.p == rhs.p && lhs.t == rhs.t);
	}

	friend bool operator!=(const Node& lhs, const Node& rhs) {
		return !(lhs.p == rhs.p && lhs.t == rhs.t);
	}

	friend bool operator<(const Node& lhs, const Node& rhs) {
		return (lhs.t < rhs.t);
	}

	friend bool operator>(const Node& lhs, const Node& rhs) {
		return (lhs.t > rhs.t);
	}

	friend bool operator<=(const Node& lhs, const Node& rhs) {
		return (lhs.t <= rhs.t);
	}

	friend bool operator>=(const Node& lhs, const Node& rhs) {
		return (lhs.t >= rhs.t);
	}*/

	~Node(){}
private:
	cv::Point thisPoint;
	list<cv::Point> conn_prev;
	list<cv::Point> conn_next;
	//int t;
};

#endif