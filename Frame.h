#ifndef FRAME_H
#define FRAME_H

#include "stdafx.h"
#include "Problem.h"
#include <sstream>

using namespace std;
using namespace cv;

class Frame {
public:
	Frame();
	Frame(Mat&);
	Mat getFrame();
	void showFrame();
	list<CBlob> getCandidates();
	int findCandidates(Mat, Mat);
	void drawCandidates();
	void narrowCandidates(CBlobResult&, CBlobResult&);
	~Frame();

private:
	Mat curr;
	list<CBlob> candidates;
};

#endif