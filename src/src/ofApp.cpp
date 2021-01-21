#include "ofApp.h"
#include "ofMain.h"
vector<waterObj> gWaters;						//����� obj���� ����
												//rank���� ����
priority_queue<pair<float, string>> rankQueue;	

int player_x = 512;								//player�� x ��ǥ
int player_y = 384;								//player�� y ��ǥ
int play = 0;									//���������� �ƴ��� ��Ÿ���� flag�̴�
int level = 1;									//������ level�� ��Ÿ����
float score = 0;								//������ score�� ��Ÿ����
int waterFlag = 0;								//���湰�� ���� ��ġ�� �����ϴ� �����̴�.
int gameOverFlag;								//GameOver���� ��Ÿ���� flag
int gameTime = 0;								//���� �ð��� ��Ÿ����
int pauseFlag = 0;								//������ �Ͻ����� �� �� �ִ� flag
int inputNameFlag = 0;							//�̸��� �Է¹޾ƾ����� ��Ÿ���� flag
string playerName = "";							//�÷��̾��� �̸��� �޴� string
queue<pair<float, string>> outputRank;			//rank.txt ���Ϸ� ����Ҷ� ����ϴ� queue
const int max_player_name_length = 15;			//�÷��̾� �̸� ������ �ִ밪

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("20181746 Park Jun Seong");	//������â �̸�
	ofSetWindowShape(1024, 768);					//������â ũ��
	ofBackground(ofColor::white);					//����
	ofSetColor(ofColor::black);						//�� ����
	srand(time(NULL));								//rand�Լ��� ���� seed
}

//--------------------------------------------------------------
void ofApp::update(){
	/*���� �÷������� �ƴҶ� update �������� ����*/
	if (!play)
		return;

	if (pauseFlag) {
		return;
	}

	/*gameOver�� �Ǹ� update �������� ����*/
	if (gameOverFlag) {
		return;
	}
	/*���ο� water ��ü ����*/
	makeWater();

	/*
	water obj���� �����Ͽ� ������ ������ ������
	��ü�� ������â ������ ������ ��� ��ü�� � ������ �ݻ��Ŵ
	�̶� �� ��ü�� ������ �ݻ�� Ƚ���� ������� popNum���� ������ �ְ�
	�ִ� ƨ�� �� �ִ� Ƚ���� level��ŭ�� ���� �� Ƚ���� ������ ������
	������ �Ǹ� level*level*0.1��ŭ�� ������ ȹ����
	*/
	for (int i = 0; i < gWaters.size(); i++) {
		if (gWaters[i].getY() > ofGetWindowHeight() || gWaters[i].getY() < 0) {		// y�� �˻�
			if (gWaters[i].getPopNum() >= level) {									// popNum == level�̸� ����
				gWaters.erase(gWaters.begin() + i);
				i--;
				score += 0.1 * level * level;
				continue;
			}
			gWaters[i].setMoveY(-gWaters[i].getMoveY());		// y���� ������� �ٲ�
			gWaters[i].setPopNum();								// popNum++
		}
		if (gWaters[i].getX() > ofGetWindowWidth() || gWaters[i].getX() < 0) {		// x�� �˻�
			if (gWaters[i].getPopNum() >= level) {									// popNum == level�̸� ����
				gWaters.erase(gWaters.begin() + i);
				i--;
				score += 0.1 * level * level;
				continue;
			}
			gWaters[i].setPopNum();
			gWaters[i].setMoveX(-gWaters[i].getMoveX());		//	x���� ����� �ٲ�
								;								// popNum++
		}

		gWaters[i].waterObj::updateWater();		// water ��ü�� �̵���Ŵ
		if (isGameOver(i)) {					// water ��ü�� play�� ������ gameOver
			gameOverFlag = 1;
			inputNameFlag = 1;
			return;
		}
	}
	score += 0.1*level;							// score ����
	gameTime = score / 1;						// score�� ���� �ð��� ���Ѵ�
												// ��Ȯ�� �ð��� �ƴ����� ������ ���� ���̵��� �ο��ϱ� ����
}
//--------------------------------------------------------------
void ofApp::draw() {
	// str = score ����� ���� �迭, str2 = level ����� ���� �迭
	char str[50];
	char str2[50];

	/* 
	���� �����ϱ����� �⺻������ �÷��̾ ����ϰ�(line 99)
	level�� ���򸻵��� ȭ�鿡 ����Ѵ� (line 100~104)
	*/
	if (!play) {
		ofDrawCircle(ofVec2f(player_x, player_y), 10);
		sprintf(str2, "Level : %d", level);
		ofDrawBitmapString(str2, 450, 80);
		ofDrawBitmapString("Press 'T' to level up", 400, 100);
		ofDrawBitmapString("Press 'Y' to level down", 400, 120);
		ofDrawBitmapString("Press 'S' to start", 400, 140);
		return;
	}

	if (pauseFlag) {
		ofDrawBitmapString("PAUSE", 512, 384);
	}

	/*
	������ ������ �̸��� �Է¹ޱ����� ��� (line 118~119)
	�̸��� �Է¹��� �� gameOver�� score (line 124~126) �� ����ϰ�
	������ ����Ѵ� (line 127~128) ���� ��ũ�� ����ϰ� ���Ͽ� ����(lin 129~130)
	*/
	if (gameOverFlag) {
		if (inputNameFlag) {
			ofDrawBitmapString("Your name ? ", 350, 364);
			ofDrawBitmapString(playerName, 470, 364);
		}
		else {
			readRankFile();
			ofDrawBitmapString("GameOver!", 350, 164);
			sprintf(str, "Score : %.2f", score);
			ofDrawBitmapString(str, 350, 184);
			ofDrawBitmapString("Press 'Q' to end the game", 350, 204);
			ofDrawBitmapString("Press 'R' to play again", 350, 224);
			printRank();
			writeRank();
		}
		return;
	}

	//�÷��̾ �׸���
	ofDrawCircle(ofVec2f(player_x, player_y), 10);

	// waterObj ���� �׸���.
	ofSetColor(ofColor::blue);
	for (int i = 0; i < gWaters.size(); i++) {
		ofDrawCircle(ofVec2f(gWaters[i].getX(), gWaters[i].getY()), 5);
	}

	//������ ������ ȭ�鿡 ����Ѵ�.
	ofSetColor(ofColor::black);
	sprintf(str, "Score : %.2f", score);
	ofDrawBitmapString(str, 900, 100);
	sprintf(str2, "Level : %d", level);
	ofDrawBitmapString(str2, 900, 80);
}

//--------------------------------------------------------------
/*
q�� ������ ���� ����
s�� ������ ���� ����
t�� ������ ���̵� ����
y�� ������ ���̵� ����
r�� ������ ���� �ʱ�ȭ������ ������
���̵� ����/���Ҵ� �������϶��� ����� �� ����
*/
void ofApp::keyPressed(int key){
	if (inputNameFlag) {
		inputName(key);
		return;
	}
	else {
		switch (key) {
		case 'Q': case 'q':
			freeMemory();
			ofExit(0);
			break;
		case 'S': case 's':
			play = 1;
			break;
		case 'T': case 't':
			if (!play) {
				if (level < 5)
					level++;
			}
			break;
		case 'Y': case 'y':
			if (!play) {
				if (level > 1)
					level--;
			}
			break;
		case 'R': case'r':
			freeMemory();
			play = 0;
			score = 0;
			level = 1;
			gameOverFlag = 0;
			player_x = 512;
			player_y = 384;
			gameTime = 0;
			pauseFlag = 0;
			inputNameFlag = 0;
			break;
		}
	}

}

//--------------------------------------------------------------
//space Ű�� ������ ������ �Ͻ������Ѵ�.
void ofApp::keyReleased(int key){
	if (play) {
		if (key == ' ') {
			pauseFlag = 1 - pauseFlag;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
//���콺 ���� ���콺�� Ŭ����ä �巡���ϸ� �� �������� �÷��̾ �̵��Ѵ�.
void ofApp::mouseDragged(int x, int y, int button){
	if (play) {
		if (pauseFlag)
			return;

		if (button == OF_MOUSE_BUTTON_LEFT) {
			player_x = x;
			player_y = y;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//�޸𸮸� ������Ų��
void ofApp::freeMemory() {
	gWaters.clear();
	playerName.clear();
	if (!rankQueue.empty())
		rankQueue.pop();
	if (!outputRank.empty()) 
		outputRank.pop();
}

/*
water Obj�� ����� �κ��̴�. ������� ���� �ִ밡 �ƴҶ� ����� ��ü���� �����ϸ�
��ġ�� ���������� ȭ�� ���κ�, ���� �κ�, ������ �κ�, �Ʒ� �κп� �����ȴ�.
�� ��ü�� �ʱ� �ӵ��� rand �Լ��� �̿��� x ����� y ���� �̵� �ӵ��� ����
0 x level ~ 1 x level ������ ������ �ʱ�ȭ�Ѵ�.
������� ���� �ִ� 100*level ���̴�.
*/
void ofApp::makeWater() {
	if (gWaters.size() >  level * (100+gameTime)) {
		return;
	}

	for (int i = 0; i < level; i++) {
		if (waterFlag == 0) {
			waterFlag = (waterFlag + 1) % 4;
			gWaters.push_back(waterObj((float)((rand() % 1004) + 10), (float)10));
		}
		else if (waterFlag == 1) {
			waterFlag = (waterFlag + 1) % 4;
			gWaters.push_back(waterObj((float)10, (float)((rand() % 1004) + 10)));
		}
		else if (waterFlag == 2) {
			waterFlag = (waterFlag + 1) % 4;
			gWaters.push_back(waterObj((float)1014, (float)((rand() % 1004) + 10)));
		}
		else if (waterFlag == 3) {
			waterFlag = (waterFlag + 1) % 4;
			gWaters.push_back(waterObj((float)((rand() % 1004) + 10), (float)758));
		}

		gWaters.back().setMoveX(((float)rand() / RAND_MAX) * level);
		gWaters.back().setMoveY(((float)rand() / RAND_MAX) * level);
	}
}

//gameOver���� �Ǵ��Ѵ�. ������ �÷��̾� �߽� ������ �Ÿ��� 10sqrt(2) ���ϸ� true�̴�.
bool ofApp::isGameOver(int i) {
	return abs(gWaters[i].getX() - player_x) < 10 && abs(gWaters[i].getY() - player_y) < 10;
}

/*
rank������ �о �켱���� ť�� �����Ѵ�

rank.txt�� ����� ���´�
score name
score name
....
score name
�����̴�
*/
void ofApp::readRankFile() {
	ifstream rank;
	rank.open("rank.txt");
	if (rank.is_open()) {
		int i = 0;
		while (!rank.eof()) {
			if (i == 20)
				break;

			float score;
			string name;
			
			rank >> score >> name;

			rankQueue.push({ score, name });
			i++;
		}
	}
	rank.close();
}

/*
ȭ�鿡 1����� ������ ���� ������������ ����Ѵ�.
�̶� ȭ�鿡 ��µǴ� rank�� ���� �ִ밪�� 20�̴�.
*/

void ofApp::printRank() {
	int i = 1;
	ofDrawBitmapString("Rank History",  600, 164);
	while (!rankQueue.empty()) {
		if (i > 20)
			break;

		string str;
		str = to_string(i) + ".  " +  rankQueue.top().second + "  " + to_string(rankQueue.top().first);
		ofDrawBitmapString(str, 600, 164 + 20 * i);
		outputRank.push({ rankQueue.top().first,rankQueue.top().second });
		rankQueue.pop();
		i++;

	}
}

/*
�̸��� �Է¹޴� �Լ��̴�.
enter�� ������ �Է��� �����Ѵ�.
���� �ƹ� �Է¾��� enter�� ������ �̸��� "None"�� �ȴ�
�̸��� ��ҹ��� a~z�����������ϸ� backspace�� ����°͵� �����Ѵ�..
	
*/

void ofApp::inputName(int key) {
	switch (key) {
	case OF_KEY_RETURN :
		if (playerName.empty()) 
			playerName = "None";
		rankQueue.push({ score, playerName });
		inputNameFlag = 0;
		break;
	case OF_KEY_BACKSPACE :
		if (!playerName.empty())
			playerName.pop_back();
		break;
	default :
		if (key >= 'a' || key <= 'Z') {
			if (playerName.size() < max_player_name_length) {
				playerName.push_back((char)key);
			}
		}
		break;
	}
}

/*
rank.txt�� outputRank�� �ִ� ������ ����Ѵ�.
�̶� outputRank�� �̹� score�� ���� ������������ ���ĵ� �����̴�.

rank.txt����  ����� ���´�
score name
score name
....
score name
�����̴�
*/
void ofApp::writeRank() {
	ofstream rank;
	rank.open("rank.txt");

	rank << outputRank.front().first << " " << outputRank.front().second;
	outputRank.pop();
	while (!outputRank.empty()) {
		rank << endl;
		rank << outputRank.front().first << " " << outputRank.front().second;
		outputRank.pop();
	}
	rank.close();
}