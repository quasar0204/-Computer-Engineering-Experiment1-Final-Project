#include "ofApp.h"
#include "ofMain.h"
vector<waterObj> gWaters;						//물방울 obj들을 저장
												//rank정보 저장
priority_queue<pair<float, string>> rankQueue;	

int player_x = 512;								//player의 x 좌표
int player_y = 384;								//player의 y 좌표
int play = 0;									//게임중인지 아닌지 나타내는 flag이다
int level = 1;									//게임의 level을 나타낸다
float score = 0;								//게임의 score를 나타낸다
int waterFlag = 0;								//물방물의 생성 위치를 결정하는 변수이다.
int gameOverFlag;								//GameOver인지 나타내는 flag
int gameTime = 0;								//게임 시간을 나타낸다
int pauseFlag = 0;								//게임을 일시정지 할 수 있는 flag
int inputNameFlag = 0;							//이름을 입력받아야함을 나타내는 flag
string playerName = "";							//플레이어의 이름을 받는 string
queue<pair<float, string>> outputRank;			//rank.txt 파일로 출력할때 사용하는 queue
const int max_player_name_length = 15;			//플레이어 이름 길이의 최대값

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("20181746 Park Jun Seong");	//윈도우창 이름
	ofSetWindowShape(1024, 768);					//윈도우창 크기
	ofBackground(ofColor::white);					//배경색
	ofSetColor(ofColor::black);						//팬 색깔
	srand(time(NULL));								//rand함수를 위한 seed
}

//--------------------------------------------------------------
void ofApp::update(){
	/*게임 플레이중이 아닐때 update 진행하지 않음*/
	if (!play)
		return;

	if (pauseFlag) {
		return;
	}

	/*gameOver가 되면 update 진행하지 않음*/
	if (gameOverFlag) {
		return;
	}
	/*새로운 water 개체 생성*/
	makeWater();

	/*
	water obj들을 점검하여 삭제와 갱신을 수행함
	개체가 윈도우창 밖으로 나갔을 경우 개체의 운동 방향을 반사시킴
	이때 각 개체는 본인이 반사된 횟수를 멤버변수 popNum으로 가지고 있고
	최대 튕길 수 있는 횟수는 level만큼임 따라서 이 횟수를 넘으면 삭제함
	삭제가 되면 level*level*0.1만큼의 점수를 획득함
	*/
	for (int i = 0; i < gWaters.size(); i++) {
		if (gWaters[i].getY() > ofGetWindowHeight() || gWaters[i].getY() < 0) {		// y값 검사
			if (gWaters[i].getPopNum() >= level) {									// popNum == level이면 삭제
				gWaters.erase(gWaters.begin() + i);
				i--;
				score += 0.1 * level * level;
				continue;
			}
			gWaters[i].setMoveY(-gWaters[i].getMoveY());		// y방향 운동방향을 바꿈
			gWaters[i].setPopNum();								// popNum++
		}
		if (gWaters[i].getX() > ofGetWindowWidth() || gWaters[i].getX() < 0) {		// x값 검사
			if (gWaters[i].getPopNum() >= level) {									// popNum == level이면 삭제
				gWaters.erase(gWaters.begin() + i);
				i--;
				score += 0.1 * level * level;
				continue;
			}
			gWaters[i].setPopNum();
			gWaters[i].setMoveX(-gWaters[i].getMoveX());		//	x방향 운동방향 바꿈
								;								// popNum++
		}

		gWaters[i].waterObj::updateWater();		// water 개체를 이동시킴
		if (isGameOver(i)) {					// water 개체와 play가 닿으면 gameOver
			gameOverFlag = 1;
			inputNameFlag = 1;
			return;
		}
	}
	score += 0.1*level;							// score 증가
	gameTime = score / 1;						// score에 따라 시간을 정한다
												// 정확한 시간은 아니지만 점수에 따라 난이도를 부여하기 위함
}
//--------------------------------------------------------------
void ofApp::draw() {
	// str = score 출력을 위한 배열, str2 = level 출력을 위한 배열
	char str[50];
	char str2[50];

	/* 
	게임 진행하기전에 기본적으로 플레이어를 출력하고(line 99)
	level과 도움말들을 화면에 출력한다 (line 100~104)
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
	게임이 끝나면 이름을 입력받기위한 출력 (line 118~119)
	이름을 입력받은 후 gameOver와 score (line 124~126) 을 출력하고
	도움말을 출력한다 (line 127~128) 이후 랭크를 출력하고 파일에 쓴다(lin 129~130)
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

	//플레이어를 그린다
	ofDrawCircle(ofVec2f(player_x, player_y), 10);

	// waterObj 들을 그린다.
	ofSetColor(ofColor::blue);
	for (int i = 0; i < gWaters.size(); i++) {
		ofDrawCircle(ofVec2f(gWaters[i].getX(), gWaters[i].getY()), 5);
	}

	//점수와 레벨을 화면에 출력한다.
	ofSetColor(ofColor::black);
	sprintf(str, "Score : %.2f", score);
	ofDrawBitmapString(str, 900, 100);
	sprintf(str2, "Level : %d", level);
	ofDrawBitmapString(str2, 900, 80);
}

//--------------------------------------------------------------
/*
q를 누르면 게임 종료
s를 누르면 게임 시작
t를 누르면 난이도 증가
y를 누르면 난이도 감소
r를 누르면 게임 초기화면으로 가진다
난이도 증가/감소는 게임중일때는 사용할 수 없다
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
//space 키를 누르면 게임을 일시정지한다.
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
//마우스 왼쪽 마우스를 클릭한채 드래그하면 그 방향으로 플레이어가 이동한다.
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

//메모리를 해제시킨다
void ofApp::freeMemory() {
	gWaters.clear();
	playerName.clear();
	if (!rankQueue.empty())
		rankQueue.pop();
	if (!outputRank.empty()) 
		outputRank.pop();
}

/*
water Obj를 만드는 부분이다. 물방울의 수가 최대가 아닐때 물방울 개체들을 생성하며
위치는 순차적으로 화면 윗부분, 왼쪽 부분, 오른쪽 부분, 아랫 부분에 생성된다.
물 개체의 초기 속도는 rand 함수를 이용해 x 방향과 y 방향 이동 속도를 각각
0 x level ~ 1 x level 사이의 값으로 초기화한다.
물방울의 수는 최대 100*level 개이다.
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

//gameOver인지 판단한다. 물방울과 플레이어 중심 사이의 거리가 10sqrt(2) 이하면 true이다.
bool ofApp::isGameOver(int i) {
	return abs(gWaters[i].getX() - player_x) < 10 && abs(gWaters[i].getY() - player_y) < 10;
}

/*
rank파일을 읽어서 우선순위 큐에 저장한다

rank.txt의 저장된 형태는
score name
score name
....
score name
형태이다
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
화면에 1등부터 점수에 대한 내림차순으로 출력한다.
이때 화면에 출력되는 rank의 수의 최대값은 20이다.
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
이름을 입력받는 함수이다.
enter를 누르면 입력을 종료한다.
만약 아무 입력없이 enter만 누르면 이름은 "None"이 된다
이름은 대소문자 a~z까지만가능하며 backspace로 지우는것도 지원한다..
	
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
rank.txt로 outputRank에 있는 정보를 출력한다.
이때 outputRank는 이미 score에 대한 내림차순으로 정렬된 상태이다.

rank.txt로의  저장된 형태는
score name
score name
....
score name
형태이다
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