// TennisTracking.cpp : main project file.

#include "stdafx.h"
#include "Start.h"
#include "Problem.h"
#include "Frame.h"
#include "Node.h"
#include "Support.h"
#include "Graph.h"
#include <map>
#include <string>
#include <sstream>
#include <math.h>

#define dth 25
#define radius 50
#define V 1

using namespace TennisTracking;
using namespace std;
using namespace cv;

/*double euclideanDist(cv::Point p1, cv::Point p2) {
	double xDist = (double)((double)p1.x - (double)p2.x);
	double yDist = (double)((double)p1.y - (double)p2.y);
	return ((double)sqrt((double)((double)xDist*(double)xDist+(double)yDist*(double)yDist)));
}*/

void readVideo(string filename, int alpha) {
	if (!filename.empty()){
		VideoCapture video(filename);
		
		if(!video.isOpened()) {
			Application::Run(gcnew Problem("not opened"));
		} else if (video.isOpened()){
			int fps = (int)video.get(CV_CAP_PROP_FPS);
			int totalFrames = (int)video.get(CV_CAP_PROP_FRAME_COUNT);
			Frame *frames;
			frames = new Frame[1000];
			Mat frame, currF, prevF, tmp, tmpg;
			int key;
			int frameNum = 0;
			int numDeleted = 0;
			// namedWindow("frame", CV_WINDOW_AUTOSIZE);
			//video >> currF;

			while( key != 'q' && frameNum < video.get(CV_CAP_PROP_FRAME_COUNT)-3)
			{
				video >> frame;

					Frame f(frame);
					frames[frameNum] = f;
					frameNum++; 
			}
			int count = 0;
			Frame *prev_it = frames;
			Frame *curr_it = frames;
			Frame *next_it = frames;
			/*
			while (key != 'q' && count < totalFrames - 1) {
				imshow("frame", (*curr_it).getFrame());
				key = waitKey(0);

				if (key == 'b' && count > 0) {
					count--;
					--curr_it;
				} else {
					count++;
					++curr_it;
				}
			}
			count = 0;
			curr_it = frames;*/

			count++;
			++curr_it;
			++next_it;
			++next_it;

			multimap<int, cv::Point> graph;
			int lol = 0;
			while (count < frameNum-10) {
				
				lol += (*curr_it).findCandidates((*prev_it).getFrame(), (*next_it).getFrame());
				//(*curr_it).drawCandidates();
				//(*curr_it).showFrame();
				//waitKey(1000/fps);
				
				list<CBlob> blobs = (*curr_it).getCandidates();
				list<CBlob>::iterator blob;

				for (blob = blobs.begin(); blob != blobs.end(); ++blob) {
					cv::Point p((blob->MinX() + (blob->MaxX() - blob->MinX())/2), (blob->MinY() + (blob->MaxY() - blob->MinY())/2));
					graph.insert(pair<int, cv::Point>(count, p));
				}

				++prev_it;
				++curr_it;
				++next_it;
				count++;
			}

			ostringstream blah;
			blah << lol;
			Application::Run(gcnew Problem(blah.str()));
			
			// finding seed triplet
			multimap<int, cv::Point>::iterator gprev_it;
			multimap<int, cv::Point>::iterator gcurr_it;
			multimap<int, cv::Point>::iterator gnext_it;
			multimap<int, cv::Point>::iterator toDelete;
			bool toBeDeleted = false;

			multimap<int, Node> seedTriplets;
			lol = 0;
			int lastFrame = (graph.rbegin())->first;

			for (count = V; count < lastFrame-V; count++) {
				gcurr_it = graph.lower_bound(count);

				while (gcurr_it != graph.upper_bound(count)) {
					toBeDeleted = true;
					Node n(gcurr_it->second);
					 
					for (gprev_it = graph.lower_bound(count-V); gprev_it != graph.upper_bound(count-V); ++gprev_it) {
						if (euclideanDist(gprev_it->second, gcurr_it->second) < (double)radius && euclideanDist(gprev_it->second, gcurr_it->second) > (double)0) {
							toBeDeleted = false;
							n.addPrevConn(gprev_it->second);
						}
					}

					for (gnext_it = graph.lower_bound(count+V); gnext_it != graph.upper_bound(count+V); ++gnext_it) {
						if (euclideanDist(gcurr_it->second, gnext_it->second) < (double)radius && euclideanDist(gcurr_it->second, gnext_it->second) > (double)0) {
							toBeDeleted = false;
							n.addNextConn(gnext_it->second);
						}
					}
					toDelete = gcurr_it;
					++gcurr_it;
					
					if (toBeDeleted) {
						graph.erase(toDelete);
					} else if (n.getPrevConn().size() > 0 && n.getNextConn().size() > 0) {
						seedTriplets.insert(pair<int, Node>(count, n));
					}
				}
			}
			blah << seedTriplets.size();
			Application::Run(gcnew Problem("seed triplets " + blah.str()));

			multimap<int, Node>::iterator st_it;
			list<cv::Point> matchPrev;
			list<cv::Point> matchNext;
			list<cv::Point>::iterator matchPrev_it;
			list<cv::Point>::iterator matchNext_it;
			
			// Debug
			curr_it = frames;
			++curr_it;
			for (count = 1; count < lastFrame-1; count++) {
				((*curr_it).getFrame()).copyTo(frame);
				st_it = seedTriplets.lower_bound(count);

				while (st_it != seedTriplets.upper_bound(count)) {
					matchPrev = st_it->second.getPrevConn();
					matchNext = st_it->second.getNextConn();
					int rad = 5;
					circle( frame, st_it->second.getPoint(), radius, Scalar(0,255,255), 2, 8, 0 );
					circle( frame, st_it->second.getPoint(), rad, Scalar(0,0,255), 2, 8, 0 );

					for (matchPrev_it = matchPrev.begin(); matchPrev_it != matchPrev.end(); ++matchPrev_it) {
						circle( frame, *matchPrev_it, rad, Scalar(0,255,0), 2, 8, 0 );
					}
					for (matchNext_it = matchNext.begin(); matchNext_it != matchNext.end(); ++matchNext_it) {
						circle( frame, *matchNext_it, rad, Scalar(255,0,0), 2, 8, 0 );
					}
					++st_it;
				}
				//imshow("frame", frame);
				//waitKey(0);
				++curr_it;
			}
			blah.str("");
			//blah << lol;
			//Application::Run(gcnew Problem(blah.str()));
			multimap<int, Support> optiModel;
			
			double C = 0, lastC = 0;
			int prevFrame = 0, nextFrame = 0;
			bool stop = false;
			cv::Point p0, p1, p2, p3, p4, a, v1, estp0, estp4;
			int f0, f1, f2, f3, f4;
			double k21, k32, k01, k41;
			double minDiff, currentDiff;
			bool cantExpandBack = false, cantExpandFront = false;

			// loop for each candidate in each frame
			for (st_it = seedTriplets.begin(); st_it != seedTriplets.end(); ++st_it) {
				matchPrev = st_it->second.getPrevConn();
				matchNext = st_it->second.getNextConn();
				assert(matchPrev.size() > 0);
				assert(matchNext.size() > 0);

				// double loop to get every combination of seed triplet
				for (matchPrev_it = matchPrev.begin(); matchPrev_it != matchPrev.end(); ++matchPrev_it) {
					for (matchNext_it = matchNext.begin(); matchNext_it != matchNext.end(); ++matchNext_it) {
						Support s(st_it->second.getPoint(), st_it->first);
						cantExpandBack = false;
						cantExpandFront = false;
						p1 = *matchPrev_it;
						p2 = st_it->second.getPoint();
						p3 = *matchNext_it;

						if (p1 != p3) {

							f2 = st_it->first;
							f1 = f2 - 1;
							f3 = f2 + 1;
							s.addPrevConn(f1, p1);
							s.addNextConn(f3, p3);

							C = 100;
							lastC = 100;
							bool stop = false;

							// loop for fitting model
							while (!stop) {
								lastC = C;
								C = 0;

								k21 = (double)((double)f2-(double)f1);
								k32 = (double)((double)f3-(double)f2);
								k01 = (double)-1;
								k41 = (double)((double)f3-(double)f1+1);

								a.x = 2*((k21*(p3.x - p2.x)-k32*(p2.x - p1.x))/(k21*k32*(k21+k32)));
								a.y = 2*((k21*(p3.y - p2.y)-k32*(p2.y - p1.y))/(k21*k32*(k21+k32)));

								v1.x = ((p2.x - p1.x)/k21) - (k21*a.x/2);
								v1.y = ((p2.y - p1.y)/k21) - (k21*a.y/2);
							
								// find p1 and p3 in seed triplets map to get p0 and p4
								multimap<int, Node>::iterator p1_it, p3_it;
								list<cv::Point> p0_candidates, p4_candidates;
								list<cv::Point>::iterator p0_it, p4_it;

								if (!cantExpandBack) {
									for (p1_it = seedTriplets.lower_bound(f1); p1_it != seedTriplets.upper_bound(f1) && p1 != p1_it->second.getPoint(); ++p1_it);
									if (p1_it == seedTriplets.end() || p1_it->first != f1) cantExpandBack = true;
									else {
										p0_candidates = p1_it->second.getPrevConn();
										estp0.x = p1.x + (k01*v1.x) + ((k01*k01/2)*a.x);
										estp0.y = p1.y + (k01*v1.y) + ((k01*k01/2)*a.y);
										p0 = cv::Point(0,0);
										minDiff = dth;

										for (p0_it = p0_candidates.begin(); p0_it != p0_candidates.end(); ++p0_it) {
											currentDiff = euclideanDist((*p0_it), estp0);
											if (currentDiff < dth) {
												C += currentDiff*currentDiff;
												if (currentDiff < minDiff) {
													minDiff = currentDiff;
													p0 = (*p0_it);
												}
											} else C += dth*dth;
										}
										if (p0 == cv::Point(0,0)) cantExpandBack = true;
										else {
											s.addPrevConn(f1-1, p0);
											p1 = p0;
											f1--;
										}
									}
								}

								if (!cantExpandFront) {
									for (p3_it = seedTriplets.lower_bound(f3); p3_it != seedTriplets.upper_bound(f3) && p3 != p3_it->second.getPoint(); ++p3_it);
									if (p3_it == seedTriplets.end() || p3_it->first != f3) cantExpandFront = true;
									else {
										p4_candidates = p3_it->second.getNextConn();
										estp4.x = p1.x + (k41*v1.x) + ((k41*k41/2)*a.x);
										estp4.y = p1.y + (k41*v1.y) + ((k41*k41/2)*a.y);
										p4 = cv::Point(0,0);
										minDiff = dth;

										for (p4_it = p4_candidates.begin(); p4_it != p4_candidates.end(); ++p4_it) {
											currentDiff = euclideanDist((*p4_it), estp4);
											if (currentDiff < dth) {
												C += currentDiff*currentDiff;
												if (currentDiff < minDiff) {
													minDiff = currentDiff;
													p4 = (*p4_it);
												}
											} else C += dth*dth;
										}
										if (p4 == cv::Point(0,0)) cantExpandFront = true;
										else {
											s.addNextConn(f3+1, p4);
											p3 = p4;
											f3++;
										}
									}
								}

								// determine if model can be expanded both ways or one way
								if ((cantExpandBack && cantExpandFront) || C > lastC) stop = true;
							}
							if (s.numConnections()>2) optiModel.insert(pair<int, Support>(st_it->first, s));
						}
					}
				}
			}
			
			blah.str("");
			blah << optiModel.size();
			Application::Run(gcnew Problem("optiModel size" + blah.str()));
			
			// Debug
			curr_it = frames;
			++curr_it;
			multimap<int, Support>::iterator om_it;
			int r = 5;
			((*curr_it).getFrame()).copyTo(frame);
			for (int z=1; z<lastFrame-1; z++) {
				((*curr_it).getFrame()).copyTo(frame);
				om_it = optiModel.lower_bound(z);
				if (om_it != optiModel.upper_bound(z))
				circle( frame, om_it->second.getPoint(), r, Scalar(0,0,255), 2, 8, 0 );

				while (om_it != optiModel.upper_bound(z)) {
					graph = om_it->second.getPrevConn();
					assert(graph.size() > 0);
					for (gprev_it = graph.begin(); gprev_it != graph.end(); ++gprev_it) {
						circle( frame, gprev_it->second, r, Scalar(0,255,0), 2, 8, 0 );
					}
					graph = om_it->second.getNextConn();
					assert(graph.size() > 0);
					for (gnext_it = graph.begin(); gnext_it != graph.end(); ++gnext_it) {
						circle( frame, gnext_it->second, r, Scalar(255,0,0), 2, 8, 0 );
					}
					++om_it;
				}
				imshow("frame", frame);
				waitKey(0);
				++curr_it;
			}
			//Application::Run(gcnew Problem("tracklet level"));
			
			Graph g(alpha);
			//multimap<int, Support>::iterator om_it;
			for (om_it = optiModel.begin(); om_it != optiModel.end(); ++om_it) {
				g.addNode(om_it->second);
			}

			//curr_it = frames;
			//((*curr_it).getFrame()).copyTo(frame);
			//g.drawNodes(frame);
			blah.str("");
			blah << g.getSize();
			Application::Run(gcnew Problem("graph size" + blah.str()));
			//Application::Run(gcnew Problem("nodes added"));
			g.fillEdges();
			//Application::Run(gcnew Problem("edges filled"));
			
			/*blah.str("");
			blah << g.getNodes().size();
			Application::Run(gcnew Problem("graphsize after edges filled" + blah.str()));
			*/
			curr_it = frames;
			((*curr_it).getFrame()).copyTo(frame);
			g.drawNodes(frame);
			//imshow("frame", frame);
			//waitKey(0);
			//Application::Run(gcnew Problem("done drawing "));
			/*
			g.find_ASSP();
			Application::Run(gcnew Problem("all paths found"));

			g.drawPaths(frame);

			vector<Path> paths = g.reducePaths();

			blah.str("");
			blah << paths.size();
			Application::Run(gcnew Problem("paths reduced to " + blah.str()));
			vector<Path>::iterator paths_it;
			vector<Support> supports;
			vector<Support>::iterator s_it;

			//Debug
			curr_it = frames;
			((*curr_it).getFrame()).copyTo(frame);

			for (paths_it = paths.begin(); paths_it != paths.end(); ++paths_it) {
				supports = (*paths_it).getSupports();
				for (s_it = supports.begin(); s_it != supports.end(); ++s_it) {
					graph = (*s_it).getEverything();
					for (gcurr_it = graph.begin(); gcurr_it != graph.end(); ++gcurr_it) {
						circle( frame, gcurr_it->second, r, Scalar(255,0,0), 2, 8, 0 );
					}
				}
			}
			imshow("result", frame);
			waitKey(0);*/
			lol = 0;
			curr_it = frames;
			++curr_it;
			vector<Support> gNodes = g.getNodes();
			multimap<int, cv::Point> everything;

			for (vector<Support>::iterator nit = gNodes.begin(); nit != gNodes.end(); ++nit) {
				multimap<int, cv::Point> everytmp = (*nit).getEverything();
				everything.insert(everytmp.begin(), everytmp.end());
			}
			multimap<int, cv::Point>::iterator everyit;
			//int r = 5;

			VideoWriter record("tennis3.avi", CV_FOURCC('D', 'I', 'V', 'X'), fps, frame.size(), true);
			if (!record.isOpened()) Application::Run(gcnew Problem("not opened"));
			
			key = 'r';
			while (key == 'r') {
				curr_it = frames;
				++curr_it;
				double maxSpeed = 0, speed;
				cv::Point point;
				for (int z=1; z<lastFrame; z++) {
					((*curr_it).getFrame()).copyTo(frame);
					maxSpeed = 0;

					for (vector<Support>::iterator nit = gNodes.begin(); nit != gNodes.end(); ++nit) {
						multimap<int, cv::Point> everytmp = (*nit).getEverything();
						if (everytmp.find(z) != everytmp.end()) {
							speed = (*nit).getSpeed();
							if (speed > maxSpeed) maxSpeed = speed;
						}
					}
					maxSpeed = ((maxSpeed/(double)80)*(double)fps)*(0.001*3600*6);
					blah.str("");
					//blah.str("Speed = ");
					blah << maxSpeed;
					putText(frame, blah.str(), cv::Point(100,50), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 3, 8);

					for (everyit = everything.lower_bound(z); everyit != everything.upper_bound(z); ++everyit) {
						point = everyit->second;
						
						circle( frame, point, 5, Scalar(0,255,0), 2, 8, 0 );
					}
					for (everyit = everything.lower_bound(z-1); everyit != everything.upper_bound(z-1); ++everyit) {
						point = everyit->second;
						if (point.y < 600 && point.y > 100 && point.x < 1110 && point.x > 194)
						circle( frame, point, 3, Scalar(0,255,255), 2, 8, 0 );
					}
					for (everyit = everything.lower_bound(z-2); everyit != everything.upper_bound(z-2); ++everyit) {
						point = everyit->second;
						if (point.y < 600 && point.y > 100 && point.x < 1110 && point.x > 194)
						circle( frame, point, 1, Scalar(0,255,255), 2, 8, 0 );
					}
					
					imshow("frame", frame);
					//record << frame;
					key = waitKey(1000/fps);

					if (key == 'b') {
						z -= 2;
						--curr_it;
					} else ++curr_it;
				}
				imshow("frame", frame);
				key = waitKey(0);
			}
			delete[] frames;
			cvDestroyWindow("frame");
			cvDestroyWindow("result");
		}
	}
}


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	int option = 0;
	char first[100];
	string file1;
	int alpha = 0;
	bool exit = false;

	while (!exit) {
		option = 0;
		alpha = 10;
		file1 = "";
		Application::Run(gcnew Start(option, first, alpha));
		file1 = first;

		if (option == 0) {
			exit = true;
		} else if (option == 1) {
			readVideo(file1, alpha);
		}
	}
	return 0;
}
