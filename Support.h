#ifndef SUPPORT_H
#define SUPPORT_H

#include "stdafx.h"
#include <map>

using namespace std;
using namespace cv;

class Support {
public:
	Support(){	}
	
	Support(cv::Point p, int frame) {
		thisPoint = p;
		t = frame;
	}

	Support(int x, int y){
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

	map<int, cv::Point> candidatesBetween(int frame1, int frame2) {
		map<int, cv::Point> candidates;
		multimap<int, cv::Point>::iterator it;

		for (it = conn_prev.begin(); it != conn_prev.end(); ++it) 
			if (it->first >= frame1 && it->first <= frame2) candidates.insert(pair<int, cv::Point>(it->first, it->second));

		for (it = conn_next.begin(); it != conn_next.end(); ++it) 
			if (it->first >= frame1 && it->first <= frame2) candidates.insert(pair<int, cv::Point>(it->first, it->second));

		if (t >= frame1 && t <= frame2) candidates.insert(pair<int, cv::Point>(t, thisPoint)); 

		return candidates;
	}

	void addPrevConn(int frame, cv::Point toAdd) {
		conn_prev.insert(pair<int, cv::Point>(frame, toAdd));
	}

	void addNextConn(int frame, cv::Point toAdd) {
		conn_next.insert(pair<int, cv::Point>(frame, toAdd));
	}

	void deletePrevConn(int frame, cv::Point toDelete) {
		multimap<int, cv::Point>::iterator deletionIt = findPrevConn(frame, toDelete);
		if (deletionIt != conn_prev.end()) conn_prev.erase(deletionIt);
	}

	void deleteNextConn(int frame, cv::Point toDelete) {
		multimap<int, cv::Point>::iterator deletionIt = findNextConn(frame, toDelete);
		if (deletionIt != conn_next.end()) conn_next.erase(deletionIt);
	}

	multimap<int, cv::Point>::iterator findPrevConn(int frame, cv::Point toEval) {
		multimap<int, cv::Point>::iterator prev_it = conn_prev.find(frame);
		if (prev_it != conn_prev.end() && prev_it->second == toEval) return prev_it;
		else return conn_prev.end();
	}

	multimap<int, cv::Point>::iterator findNextConn(int frame, cv::Point toEval) {
		multimap<int, cv::Point>::iterator next_it = conn_next.find(frame);
		if (next_it != conn_next.end() && next_it->second == toEval) return next_it;
		else return conn_next.end();
	}

	int numConnections() {
		return (conn_prev.size() + conn_next.size());
	}

	multimap<int, cv::Point> getPrevConn() {
		return conn_prev;
	}

	multimap<int, cv::Point> getNextConn() {
		return conn_next;
	}

	pair<int, cv::Point> getFirst() {
		return *(conn_prev.begin());
	}

	pair<int, cv::Point> getLast() {
		return *(conn_next.rbegin());
	}
	
	friend bool operator==(const Support& lhs, const Support& rhs) {
		bool equals = false;
		
		if (lhs.thisPoint == rhs.thisPoint) {
			multimap<int, cv::Point>::const_iterator lhs_it = (lhs.conn_prev).begin();
			multimap<int, cv::Point>::const_iterator rhs_it = (rhs.conn_prev).begin();
			equals = true;

			while (lhs_it != lhs.conn_prev.end() && rhs_it != rhs.conn_prev.end() && equals) {
				if (lhs_it->first != rhs_it->first || lhs_it->second != rhs_it->second) equals = false;
				++lhs_it;
				++rhs_it;
			}
			lhs_it = (lhs.conn_next).begin();
			rhs_it = (rhs.conn_next).begin();

			while (lhs_it != lhs.conn_next.end() && rhs_it != rhs.conn_next.end() && equals) {
				if (lhs_it->first != rhs_it->first || lhs_it->second != rhs_it->second) equals = false;
				++lhs_it;
				++rhs_it;
			}
		}
		return equals;
	}

	multimap<int, cv::Point> getEverything() {
		multimap<int, cv::Point> everything;

		everything = conn_prev;
		everything.insert(conn_next.begin(), conn_next.end());
		everything.insert(pair<int, cv::Point>(t, thisPoint));
		return everything;
	}

	pair<int, int> yRange() {
		int maxY = thisPoint.y;
		int minY = thisPoint.y;
		multimap<int, cv::Point>::iterator everything;

		for (everything = conn_prev.begin(); everything != conn_prev.end(); ++everything) {
			if (everything->second.y > maxY) maxY = everything->second.y;
			if (everything->second.y < minY) minY = everything->second.y;
		}
		for (everything = conn_next.begin(); everything != conn_next.end(); ++everything) {
			if (everything->second.y > maxY) maxY = everything->second.y;
			if (everything->second.y < minY) minY = everything->second.y;
		}
		return pair<int, int>(minY, maxY);
	}

	/*
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

	double getSpeed() {
		cv::Point p1 = conn_prev.begin()->second, p2 = conn_next.rbegin()->second;
		int f1 = conn_prev.begin()->first, f2 = conn_next.rbegin()->first;
		double xDist = (double)((double)p1.x - (double)p2.x);
		double yDist = (double)((double)p1.y - (double)p2.y);
		return ((double)sqrt((double)((double)xDist*(double)xDist+(double)yDist*(double)yDist))/(double)(f2-f1));
	}

	~Support(){}
private:
	cv::Point thisPoint;
	multimap<int, cv::Point> conn_prev;
	multimap<int, cv::Point> conn_next;
	int t;
};

#endif