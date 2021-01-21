#pragma once

#include "ofMain.h"
#include "waterObj.h"
#include <vector>
#include <fstream>
#include <queue>
#include <cstdlib>
#include <ctime>
#define EPSILON 1


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void freeMemory();
		void makeWater();
		bool isGameOver(int);

		void readRankFile();
		void printRank();
		void writeRank();
		void inputName(int);

};
