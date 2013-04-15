#include "stdafx.h"
#include "Frame.h"

using namespace TennisTracking;
Frame::Frame(){
	
}

Frame::Frame(Mat& tmp) {
	tmp.copyTo(curr);
}

Mat Frame::getFrame() {
	return curr;
}

void Frame::showFrame() {
	imshow("frame", curr);
}

list<CBlob> Frame::getCandidates() {
	return candidates;
}

int Frame::findCandidates(Mat prev, Mat next) {
	Mat diff1, diff2;
	Mat gray_diff1, gray_diff2;
	Mat andMaskGray, andMaskHSV, andMaskBGR1, andMaskBGR2, andMaskBGR;
	Mat colourFiltered1;
		
	absdiff(prev, curr, diff1);
	absdiff(curr, next, diff2);

 	cvtColor(diff1, gray_diff1, CV_BGR2GRAY);
	cvtColor(diff2, gray_diff2, CV_BGR2GRAY);

	bitwise_and(gray_diff1, gray_diff2, andMaskGray);
	
	GaussianBlur( andMaskGray, andMaskGray, cv::Size(9, 9), 2, 2 );
	//morphologyEx(andMaskGray, andMaskGray, MORPH_CLOSE, getStructuringElement(MORPH_RECT, cv::Size(11, 11)));
	//morphologyEx(andMaskGray, andMaskGray, MORPH_OPEN, getStructuringElement(MORPH_RECT, cv::Size(5, 5)));
	//dilate(andMaskGray, andMaskGray, Mat());
	//GaussianBlur( andMaskGray, andMaskGray, cv::Size(9, 9), 2, 2 );

	threshold(andMaskGray, andMaskGray, 4, 255, THRESH_BINARY);
	GaussianBlur( andMaskGray, andMaskGray, cv::Size(9, 9), 2, 2 );
	threshold(andMaskGray, andMaskGray, 4, 255, THRESH_BINARY);
	//GaussianBlur( andMaskGray, andMaskGray, cv::Size(9, 9), 2, 2 );
	//threshold(andMaskGray, andMaskGray, 5, 255, THRESH_BINARY);

	// colour
	//cvtColor(curr, andMaskHSV, CV_BGR2HSV);
	//Scalar min(0.11*256, 0.4*256, 0.5*256, 0);
	//Scalar max(0.15*256, 0.7*256, 1*256, 0);
	//inRange(andMaskHSV, min, max, andMaskBGR);
	//Scalar min(140, 150, 130, 0);
	//Scalar max(200, 255, 180, 0);
	Scalar min1(90, 200, 160, 0);
	Scalar max1(175, 255, 220, 0);
	inRange(curr, min1, max1, andMaskBGR1);
	Scalar min2(28, 90, 35, 0);
	Scalar max2(100, 120, 120, 0);
	inRange(curr, min2, max2, andMaskBGR2);
	bitwise_or(andMaskBGR1, andMaskBGR2, andMaskBGR);
	GaussianBlur( andMaskBGR, andMaskBGR, cv::Size(9, 9), 2, 2 );
	morphologyEx(andMaskBGR, andMaskBGR, MORPH_CLOSE, getStructuringElement(MORPH_RECT, cv::Size(11, 11)));
	morphologyEx(andMaskBGR, andMaskBGR, MORPH_OPEN, getStructuringElement(MORPH_RECT, cv::Size(5, 5)));

	threshold(andMaskBGR, andMaskBGR, 1, 255, THRESH_BINARY);
	bitwise_and(andMaskBGR, andMaskGray, colourFiltered1);
	
	/*vector<Vec3f> circles;
	HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, colourFiltered1->rows/4, 200, 100 );
	for( size_t i = 0; i < circles.size(); i++ )
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// draw the circle center
		circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
		// draw the circle outline
		circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
	}*/

	//Mat tmp;
	//cvtColor(andMaskHSV, tmp, CV_HSV2BGR);
	//imshow("bgr", tmp);
	//imshow("hsv", andMaskHSV);
	//imshow("curr", curr);
	//imshow("andmask", andMaskGray);
	//imshow("colours", andMaskBGR);
	//imshow("thresholded", colourFiltered1);
	//waitKey(0);
	IplImage blobimage = andMaskGray;

	CBlobResult results = CBlobResult(&blobimage, NULL, 0);
	results.Filter(results, B_EXCLUDE, CBlobGetArea(), B_GREATER, 500);
	results.Filter(results, B_EXCLUDE, CBlobGetArea(), B_LESS, 200);

	CBlob *blob;
				 
	//Blob numbering
	int i;
	for(i=0; i<results.GetNumBlobs(); i++) {
		blob = results.GetBlob(i);
					
		//if((blob->MaxY() - blob->MinY()) < 1*(blob->MaxX() - blob->MinX())) {
			candidates.push_back(*blob);
		//}
	}
	return i;
}

void Frame::drawCandidates() {
	list<CBlob>::iterator blob;
	Mat copy;
	curr.copyTo(copy);

	for(blob = candidates.begin(); blob != candidates.end(); ++blob) {
		cv::Point center(blob->MinX() + (blob->MaxX() - blob->MinX())/2, blob->MinY() + (blob->MaxY() - blob->MinY())/2);
		int radius = 5;
		circle( copy, center, radius, Scalar(0,0,255), 2, 8, 0 );
	}
	imshow("copy", copy);
	waitKey(0);
}

/*void Frame::narrowCandidates(CBlobResult& prev, CBlobResult& next) {
	CBlob *blob_curr;

	for (int i=0; i<candidates.GetNumBlobs(); i++) {
		blob = candidates.GetBlob(i);

		CBlob *blob_prev, *blob_next;

		for (int j=0; j<prev.GetNumBlobs(); j++) {

		}
	}
}*/

Frame::~Frame() {}