#ifndef GRAPH_H
#define GRAPH_H

#include "stdafx.h"
#include "Problem.h"
#include "Support.h"
#include "Path.h"
#include <queue>
#include <map>
#include <vector>
#include <sstream>

#define MAXDIST 100000

using namespace TennisTracking;
using namespace std;
using namespace cv;

double euclideanDist(cv::Point p1, cv::Point p2) {
	double xDist = (double)((double)p1.x - (double)p2.x);
	double yDist = (double)((double)p1.y - (double)p2.y);
	return ((double)sqrt((double)((double)xDist*(double)xDist+(double)yDist*(double)yDist)));
}

class Graph {
public:
	Graph() {
		size = 0;
	}

	Graph(int a) {
		alpha = a;
	}
	
	void addNode(Support b) {
		if (b.numConnections() > 2) {
			vector<Support>::iterator a;
			int aFirstFrame, aLastFrame, bFirstFrame, bLastFrame;
			cv::Point aFirstPoint, aLastPoint, bFirstPoint, bLastPoint;
			bFirstFrame = b.getFirst().first;
			bFirstPoint = b.getFirst().second;
			bLastFrame = b.getLast().first;
			bLastPoint = b.getLast().second;
			bool overlap, single = true;
			
			for (a = nodes.begin(); a != nodes.end(); ++a) {
				aFirstFrame = (*a).getFirst().first;
				aFirstPoint = (*a).getFirst().second;
				aLastFrame = (*a).getLast().first;
				aLastPoint = (*a).getLast().second;

				// if a is before b but both overlap
				if (bFirstFrame > aFirstFrame && bLastFrame > aLastFrame && bFirstFrame <= aLastFrame) {
					map<int, cv::Point> overlapA = (*a).candidatesBetween(bFirstFrame, aLastFrame);
					map<int, cv::Point> overlapB = b.candidatesBetween(bFirstFrame, aLastFrame);
					overlap = true;
					

					for (int j = bFirstFrame; j <= aLastFrame && overlap; j++) {
						if (overlapA.find(j) == overlapA.end() && overlapB.find(j) != overlapB.end()) overlap = false;
						else if (overlapA.find(j) != overlapA.end() && overlapB.find(j) == overlapB.end()) overlap = false;
						else if (overlapA.find(j) != overlapA.end() && overlapB.find(j) != overlapB.end()) {
							if (overlapA.find(j)->second != overlapB.find(j)->second) {
								overlap = false;
							}
						}
					}
					
					if (overlap) {
						multimap<int, cv::Point> restOfB = b.getEverything();
						for (int i=aLastFrame+1; i<=bLastFrame; i++) {
							if (restOfB.find(i) != restOfB.end()) {
								(*a).addNextConn(i, restOfB.find(i)->second);
								size++;
							}
						}
						single = false;
					}
				} 
				// if b is before a but both overlap
				else if (aFirstFrame > bFirstFrame && aLastFrame > bLastFrame && aFirstFrame <= bLastFrame) {
					map<int, cv::Point> overlapA = (*a).candidatesBetween(aFirstFrame, bLastFrame);
					map<int, cv::Point> overlapB = b.candidatesBetween(aFirstFrame, bLastFrame);
					overlap = true;
					

					for (int j = aFirstFrame; j <= bLastFrame && overlap; j++) {
						if (overlapA.find(j) == overlapA.end() && overlapB.find(j) != overlapB.end()) overlap = false;
						else if (overlapA.find(j) != overlapA.end() && overlapB.find(j) == overlapB.end()) overlap = false;
						else if (overlapA.find(j) != overlapA.end() && overlapB.find(j) != overlapB.end()) {
							if (overlapA.find(j)->second != overlapB.find(j)->second) {
								overlap = false;
							}
						}
					}
					
					if (overlap) {
						multimap<int, cv::Point> restOfB = b.getEverything();
						for (int i=bFirstFrame; i<aFirstFrame; i++) {
							if (restOfB.find(i) != restOfB.end()) {
								(*a).addPrevConn(i, restOfB.find(i)->second);
								size++;
							}
						}
						single = false;
					}
				}
			}
			if (single && b.numConnections() > 3) {
				nodes.push_back(b);
				size += b.numConnections();
			}
		}
	}

	void fillEdges() {
		vector<Support>::iterator ita, itb, itc;
		
		int aFirstFrame, aLastFrame, bFirstFrame, bLastFrame;
		cv::Point aFirstPoint, aLastPoint, bFirstPoint, bLastPoint;
		int index1 = 0, index2 = 0;
		int frameDiff;
		double weight;
		bool overlap = true;

		ostringstream k;
		int c = 0, conflicts = 0;

		vector<double> tmp (nodes.size(), -1);

		for (unsigned int i=0; i<nodes.size(); i++) {
			connections.push_back(tmp);
		}

		index1 = 0;
		for (ita = nodes.begin(); ita != nodes.end(); ++ita) {
			index2 = 0;

			for (itb = nodes.begin(); itb != nodes.end(); ++itb) {
				aFirstFrame = (*ita).getFirst().first;
				aFirstPoint = (*ita).getFirst().second;
				aLastFrame = (*ita).getLast().first;
				aLastPoint = (*ita).getLast().second;
				bFirstFrame = (*itb).getFirst().first;
				bFirstPoint = (*itb).getFirst().second;
				bLastFrame = (*itb).getLast().first;
				bLastPoint = (*itb).getLast().second;

				frameDiff = bFirstFrame - aLastFrame;

				// if b starts after a finishes, add connection b to a
				if (frameDiff > 0 && frameDiff <= 20) {
					weight = euclideanDist(aLastPoint, bFirstPoint);
					connections[index1][index2] = weight;
					c++;
				}
				index2++;
			}
			index1++;
		}
	}

	void find_SSSP(int start) {
		priority_queue< pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > queue;
		vector<bool> visited;
		//Application::Run(gcnew Problem("starting dijkstra"));
		int current = start, child;
		double cost;
		
		visited.assign(nodes.size(), false);
		shortestDists[start][start] = 0;
		parents[start][start] = start;
		queue.push(pair<double, int>(0, start));

		while (!queue.empty()) {
			current = queue.top().second;
			queue.pop();

			if (!visited[current]) {
				visited[current] = true;

				for (child = 0; child < nodes.size(); child++) {
					cost = shortestDists[start][current] + connections[current][child];

					if (!visited[child] && connections[current][child] > -1 && cost < shortestDists[start][child]) {
						shortestDists[start][child] = cost;
						parents[start][child] = current;
						queue.push(pair<double, int>(cost, child));
					}
				}
			}
		}
	}

	void find_ASSP() {

		vector<int> p(nodes.size(), -1);
		for (int i=0; i<nodes.size(); i++) parents.push_back(p);

		vector<double> d(nodes.size(), MAXDIST);
		for (int i=0; i<nodes.size(); i++) shortestDists.push_back(d);

		for (int startNode = 0; startNode < nodes.size(); startNode++) {
			find_SSSP(startNode);
			//Application::Run(gcnew Problem("finished dijkstra"));

			for (int endNode = 0; endNode < nodes.size(); endNode++) {
				if (endNode != startNode) {
					//if ((int)connections[startNode][endNode] > -1) {
						vector<Support> s;
						double weight;
						int e = endNode;
						int numSupports = 0;

						while (e > -1 && parents[startNode][e] != startNode) {
							//Application::Run(gcnew Problem("pushing"));
							s.push_back(nodes[e]);
							//Application::Run(gcnew Problem("pushed"));
							e = parents[startNode][e];
							numSupports++;
						}
						s.push_back(nodes[startNode]);
						//Application::Run(gcnew Problem("done pushing"));
						if (numSupports > 3) {
							pair<int, int> yRange = (nodes[startNode]).yRange();
							int minY = yRange.first, maxY = yRange.second;
							vector<Support>::iterator it;
							
							for (it = s.begin(); it != s.end(); ++it) {
								yRange = (*it).yRange();
								if (yRange.first < minY) minY = yRange.first;
								if (yRange.second > maxY) maxY = yRange.second;
							}
							
							//if (maxY - minY > 20) {
								//Application::Run(gcnew Problem("initialising p"));
								Path p(s, shortestDists[startNode][endNode], alpha);
								paths.push_back(p);
							//}
						}
					//}
				}
			}
			//Application::Run(gcnew Problem("finished one finding"));
		}
	}

	vector<Path> reducePaths() {
		vector<Path>::iterator max, it;
		vector<Path> best;
		Path pStar;
		bool allCompatible;

		while (!paths.empty()) {
			max = paths.begin();

			for (it = paths.begin(); it != paths.end(); ++it) {
				if (*it > *max) max = it;
			}

			pStar = *max;
			paths.erase(max);
			allCompatible = true;

			for (it = best.begin(); it != best.end() && allCompatible; ++it) {
				if (!pStar.isCompatibleWith(*it)) allCompatible = false;
			}
			if (allCompatible) {
				best.push_back(pStar);
			}
		}
		return best;
	}

	void drawNodes(Mat frame) {
		vector<Support>::iterator n;
		multimap<int, cv::Point> map;
		multimap<int, cv::Point>::iterator mapit;
		Mat tmp;
		frame.copyTo(tmp);
		int k = 0;
		cv::Point point;
		for (n = nodes.begin(); n != nodes.end(); ++n) {
			//frame.copyTo(tmp);
			map = (*n).getEverything();
			for (mapit = map.begin(); mapit != map.end(); ++mapit) {
				point = mapit->second;
					if (point.y < 600 && point.y > 100 && point.x < 1110 && point.x > 194) {
				circle( tmp, point, 5, Scalar(0,0,255), 2, 8, 0 );
				k++;
					}
			}
			//imshow("nodes", tmp);
			//waitKey(0);
		}
		imshow("nodes", tmp);
			waitKey(0);
			ostringstream os;
			os << k;
			Application::Run(gcnew Problem("reduced to " + os.str()));
	}

	void drawPaths(Mat frame) {
		vector<Path>::iterator p;
		vector<Support>::iterator n;
		multimap<int, cv::Point> map;
		multimap<int, cv::Point>::iterator mapit;
		Mat tmp;

		for (p = paths.begin(); p != paths.end(); ++p) {
			vector<Support> nodesInP = (*p).getSupports();
			frame.copyTo(tmp);
			for (n = nodesInP.begin(); n != nodesInP.end(); ++n) {
				map = (*n).getEverything();
				for (mapit = map.begin(); mapit != map.end(); ++mapit) {
					circle( tmp, mapit->second, 5, Scalar(255,0,0), 2, 8, 0 );
				}
			}
			imshow("paths", tmp);
			waitKey(0);
		}
	}

	vector<Support> getNodes() {
		return nodes;
	}

	int getSize() {
		return size;
	}
	
	~Graph() {
	}
private:
	vector< Support > nodes;
	vector< vector< double > > connections;
	vector< vector< double > > shortestDists;
	vector< vector< int > > parents;
	vector<Path> paths;
	int alpha;
	int size;
};

#endif