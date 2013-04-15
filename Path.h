#ifndef PATH_H
#define PATH_H

#include "stdafx.h"
#include "Support.h"
#include <list>
#include <math.h>

using namespace std;
using namespace cv;

class Path {
public:
	Path() {}

	Path(vector<Support> s, double w, int a) {
		supports = s;
		weight = w;
		alpha = a;
		length = 0;

		for (vector<Support>::iterator it = s.begin(); it != s.end(); ++it) {
			length += (*it).numConnections() + 1;
		}
	}

	vector<Support> getSupports() {
		return supports;
	}

	bool isCompatibleWith(Path other) {
		bool compatible = true;
		vector<Support>::iterator support1, support2;
		multimap<int, cv::Point> everything1, everything2;
		multimap<int, cv::Point>::iterator everything1it, everything2it;

		for (support1 = supports.begin(); support1 != supports.end() && compatible; ++support1) {
			everything1 = (*support1).getEverything();
			
			for (support2 = other.supports.begin(); support2 != other.supports.end() && compatible; ++support2) {
				everything2 = (*support2).getEverything();

				for (everything1it = everything1.begin(); everything1it != everything1.end() && compatible; ++everything1it) {
					for (everything2it = everything2.begin(); everything2it != everything2.end() && compatible; ++everything2it) {
						if (everything1it->first == everything2it->first && everything1it->second == everything2it->second) compatible = false;
					}
				}
			}
		}
		return compatible;
	}

	friend bool operator>(const Path& lhs, const Path& rhs) {
		bool morethan = false;

		if ((lhs.weight - rhs.weight) < 1*(lhs.length - rhs.length)) {
			morethan = true;
		}
		return morethan;
	}

	friend bool operator<(const Path& lhs, const Path& rhs) {
		bool lessthan = false;

		if ((lhs.weight - rhs.weight) > 1*(lhs.length - rhs.length)) {
			lessthan = true;
		}
		return lessthan;
	}

	friend bool operator==(const Path& lhs, const Path& rhs) {
		bool equals = false;

		if ((lhs.weight - rhs.weight) == lhs.alpha*(lhs.length - rhs.length)) {
			equals = true;
		}
		return equals;
	}

	friend bool operator>=(const Path& lhs, const Path& rhs) {
		bool morethan = false;

		if ((lhs.weight - rhs.weight) <= 1*(lhs.length - rhs.length)) {
			morethan = true;
		}
		return morethan;
	}

	friend bool operator<=(const Path& lhs, const Path& rhs) {
		bool lessthan = false;

		if ((lhs.weight - rhs.weight) >= 1*(lhs.length - rhs.length)) {
			lessthan = true;
		}
		return lessthan;
	}

	~Path(){}
private:
	vector<Support> supports;
	double weight;
	int length;
	int alpha;
};

#endif