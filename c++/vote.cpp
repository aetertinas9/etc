#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

using namespace std;

#define MAX_SIZE 1000
char inputString[MAX_SIZE];


typedef struct _finddata_t  FILE_SEARCH;
void ProgressVote(){};


//파일을 포함한 디렉토리 삭제를 위한 rmdirs함수
//안에 member, group, vote 등 여러 텍스트 파일 또는 디렉토리가 있어도 디렉토리를
// system을 사용하지 않고 구현하였다
// 2017/06/21 최종수정

int rmdirs(const char *path, int is_error_stop) {
	DIR *dir_ptr = NULL;
	struct dirent *file = NULL;
	struct stat buf;
	char filename[1024];

	if((dir_ptr = opendir(path)) == NULL) {
		return unlink(path);
	}

	while((file = readdir(dir_ptr)) != NULL) {
		if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
			continue;
		}

		sprintf(filename, "%s%s", path, file->d_name);

		if(lstat(filename, &buf) == -1) {
			continue;
		}

		if(S_ISDIR(buf.st_mode)) {
			if(rmdirs(filename, is_error_stop) == -1 && is_error_stop) {
				return -1;
			}
		}else if(S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) {
			if(unlink(filename) == -1 && is_error_stop) {
				return -1;
			}
		}
	}

	closedir(dir_ptr);

	cout << "path: " << path << endl;

	return rmdir(path);

}

// Class : Session
// 싱글톤으로 구현한 세션 클래스입니다
// 2017/06/21 최종수정

class Session {

private:
        string ID;
        string group;
        Session() {
                ID = "";
                group = "";
        }

        static Session* inst;

public:

        static Session* getInstance();

        void printSession() { // 세션 정보 출력
                cout << "[Session]ID: " << ID << ", group: " << group << endl;
        }

        void updateSession(string IDInput, string groupInput) { // 세션 정보 업데이트
                ID = IDInput;
                group = groupInput;
        }

	void updateSession(string groupInput) { // 세션 정보 업데이트(그룹정보만)
		group = groupInput;
	}

        void destroySession() { // 세션 정보 삭제
                ID = "";
                group = "";
        }

        bool isAlreadyLogin() { //로그인 여부 확인
                if(ID.compare("") == 0)
                        return false;
                else
                        return true;
        }

	string getID() {	//아이디를 가져온다
		return ID;
	}

	string getGroup() { //그룹을 가져온다
		return group;
	}

};

Session* Session::inst = NULL;
Session* Session::getInstance() {
        if(inst == NULL)
                inst = new Session();

        return inst;
}
//현재시간을 알기 위한 함수
int getTimeT(int year, int month, int day, int hour, int minute, int second) {
	struct tm t = {0};
	t.tm_year = year - 1900;
	t.tm_mon = month - 1;
	t.tm_mday = day;
	t.tm_hour = hour;
	t.tm_min = minute;
	t.tm_sec = second;

	return mktime(&t);
}

void makeVote(){//투표생성
	string group = Session::getInstance()->getGroup();

	string voteName;
	int itemNumber;
	//투표주제입력
	cout << "투표의 주제를 입력 : " ;
	cin >> voteName;
	//투표항목입력
	cout << "투표 항목의 개수를 입력 : ";
	cin >> itemNumber;

	string voteItem[itemNumber];

	for(int i=0; i<itemNumber; i++)
	{
		cout<<"투표의 항목을 입력 : ";
		cin>>voteItem[i];
	}

	int year1, year2, month1, month2, day1, day2, hour1, hour2, min1, min2, sec1, sec2, date1, date2, time1, time2;

	//시작시간입력
	cout << "시작시간 입력 - 년월일(20170620) : ";
	cin >> date1;
	cout << "시작시간 입력 - 시분초(231159) : ";
	cin >> time1;
	//종료를 위한 투표 시작 시간 입력
	year1 = date1 / 10000;
	month1 = (date1 - (year1 * 10000)) / 100;
	day1 = date1 % 100;
	hour1 = time1 / 10000;
	min1 = (time1 - (hour1 * 10000)) / 100;
	sec1 = time1 % 100;

	//마감시간입력
	cout << "마감시간 입력 - 년월일(20170620) : ";
	cin >> date2;
	cout << "마감시간 입력 - 시분초(231159) : ";
	cin >> time2;

	year2 = date2 / 10000;
	month2 = (date2 - (year2 * 10000)) / 100;
	day2 = date2 % 100;
	hour2 = time2 / 10000;
	min2 = (time2 - (hour2 * 10000)) / 100;
	sec2 = time2 % 100;

	time_t startTime = getTimeT(year1, month1, day1, hour1, min1, sec1);
	time_t endTime = getTimeT(year2, month2, day2, hour2, min2, sec2);

	//파일생성
	string dirName = "./group/" + group + "/";
	string path = dirName + voteName;

	string  voteItemPath[itemNumber];
	for(int i=0; i<itemNumber; i++)
		voteItemPath[i]="";

	char * cPath = strdup( path.c_str() );

	if( mkdir(cPath, 0777) == 0 ){
		for( int i=0; i<itemNumber; i++ ){
			voteItemPath[i] = path + "/" + voteItem[i];

			cPath = strdup(voteItemPath[i].c_str());

			if( mkdir(cPath, 0777) == 0 ){
				string configFile = path + "/config";
				ofstream outFile( configFile.c_str() );
				outFile << startTime << endl; // 첫줄 시작시간
				outFile << endTime << endl; // 둘째줄 마감시간
				outFile.close();
			}
		}
		cout << "투표 생성 완료" << endl;
	}
	else{
		perror("투표 제안 실패 \n");
		if(errno == EEXIST)
			cout<<"같은 주제의 투표가 이미 존재합니다."<<endl;	//중복투표 방지
	}
}

//디렉토리에 접근하여 투표리스트를 가져온다
//최종 수정 2017/06/21

void getVoteList(){
	string group = Session::getInstance()->getGroup();
	string dirName = "./group/" + group;
	char * cDirName = strdup(dirName.c_str());

	DIR * dirInfo;
	struct dirent * dirEntry;
	string str1 = ".", str2 = "..";

	dirInfo = opendir(cDirName);

	if(dirInfo != NULL){
		while(dirEntry = readdir(dirInfo)){
			string entryStr(dirEntry->d_name);
			if(!(entryStr.compare(str1)==0 || entryStr.compare(str2)==0 || entryStr.compare("config")==0)){
				cout << entryStr << " : ";
				string voteName = dirEntry->d_name + "/" +entryStr;

		}
		closedir(dirInfo);
	}


}
}

//싱글톤 세션 이용
//


void Vote(){
	string userID = Session::getInstance()->getID();
	string group = Session::getInstance()->getGroup();
	string voteName;
	string itemName;

	cout << "다음 중 투표하려는 주제를 입력하세요." << endl;
	getVoteList();
	cout << "입력 : ";
	cin >> voteName;

	cout << "다음 중 원하는 항목에 투표하세요." << endl;

	string dirName = "./group/" + group + "/" + voteName;

	DIR * dirInfo;
	struct dirent * dirEntry;
	char * cDirName = strdup(dirName.c_str());

	dirInfo = opendir(cDirName);
	if(dirInfo != NULL){
		while(dirEntry = readdir(dirInfo)){
			string entryStr(dirEntry->d_name);
			if(!(entryStr.compare(".")==0 || entryStr.compare("..")==0 || entryStr.compare("config")==0))
				cout << entryStr << endl;
		}
		closedir(dirInfo);
	}

	cout << "항목 입력: ";
	cin >> itemName;
	string path = dirName + "/" + itemName + "/" + userID;
	char * cPath = strdup(path.c_str());
	if(access(cPath, F_OK)==0){
		//중복투표 방지
		cout<<"한 회원은 한번만 투표할 수 있습니다."<<endl;
		return;
	}
	else{
		ofstream outFile(path.c_str());
		outFile.close();
		cout << "투표 완료"<<endl;
	}
}


void guestlogin(){
	Session::getInstance()->updateSession("GUEST","");
	//제한된 기능을 가진 게스트로그인을 위한 함수
}


bool login() { // 로그인

	string ID, pass;

	cout << "ID: ";
	cin >> ID;
	cout << "Password: ";
	cin >> pass;

	string dirName = "./user/";
	string path = dirName + ID;

	char *cPath = strdup(path.c_str());

  system("clear");

//아이디가 존재하는 지 파일을 체크하고
//아이디가 가입된 회원이고 아이디 비밀번호가 일치하는지 확인
//가입한 그룹이 있는지 확인
//최종수정 2017/06/21


	if(access(cPath, F_OK) == 0) {
		ifstream ifile(path.c_str());
		if(ifile.is_open()) {
			if(ifile.getline(inputString, MAX_SIZE)) {
				string compareStr(inputString);
				if(compareStr.compare(pass) == 0) {
					cout << "로그인 성공." << endl;
					if(ifile.getline(inputString, MAX_SIZE)) { // 가입한 그룹이 있는 경우
						Session::getInstance()->updateSession(ID, inputString);
					}else {
						Session::getInstance()->updateSession(ID, "");
					}
					ifile.close();
					return true;
				}else {
					cout << "패스워드가 일치하지 않음." << endl;
					ifile.close();
					return false;
				}
			}
		}

	}else {
		cout << "존재하는 아이디가 없음." << endl; //회원가입하지 않은 아이디 일때
		return false;
	}
}

//싱글톤을 이용한 세션이용
//로그아웃을 구현한 함수

void logout() {

        Session::getInstance()->destroySession();
        cout << "로그아웃 됨" << endl;
}


void withdrawl(){//회원탈퇴

	string ID = Session::getInstance()->getID();
	string userPath = "./user/" + ID;
	//탈퇴를 원하는 현재 접속중인 아이디에 접근
	char *cPath = strdup(userPath.c_str());

	if(access(cPath,F_OK)== 0){
	remove(cPath);//회원탈퇴
	}

	Session::getInstance()->updateSession("");

}

void signUp() { // 회원가입

	string ID, pass;

	cout << "ID: ";
	cin >> ID;
	cout << "Password: ";
	cin >> pass;

	string dirName = "./user/";
	string path = dirName + ID;

  system("clear");


	char *cPath = strdup(path.c_str());

	if(access(cPath, F_OK) == 0) { // 파일 체크
		cout << "이미 존재하는 아이디가 있음." << endl;
	}else { // 새로운 파일 만들기
		ofstream outFile(path.c_str());
		outFile << pass;
		outFile << endl;
		outFile.close();
	}
}

void joinGroup() { // 그룹 가입

	string group = Session::getInstance()->getGroup();

	if(group.compare("") != 0) {
		cout << "이미 가입한 그룹이 있음." << endl;
		return;
	}

	string groupName;
	cout << "가입하기 싫음 -> 0" << endl;
	cout << "가입할 그룹 이름: ";
	cin >> groupName;

	if(groupName.compare("0") == 0) {
		cout << "가입하지 않음." << endl;
		return;
	}

	string dirName = "./group/";
	string path = dirName + groupName;

	char *cPath = strdup(path.c_str());

	if(access(cPath, F_OK) == 0) { // 파일 존재 여부 체크
		string ID = Session::getInstance()->getID();
		string userPath = "./user/" + ID;
		ofstream outFile;
		outFile.open(userPath.c_str(), std::ios_base::out | std::ios_base::app);
		outFile << groupName;
		outFile.close();
		Session::getInstance()->updateSession(groupName);
	}else { // 해당그룹이 존재 하지 않아 그룹 가입에 실패하였다
		cout << "해당 그룹이 없습니다." << endl;
	}
}

void joinGroup(string groupName) { // 그룹 생성 후 자동 가입을 위한 오버로딩

	string dirName = "./group/";
	string path = dirName + groupName;

	char *cPath = strdup(path.c_str());

	if(access(cPath, F_OK) == 0) { // 파일 존재 여부 체크
		string ID = Session::getInstance()->getID();
		string userPath = "./user/" + ID;
		ofstream outFile;
		outFile.open(userPath.c_str(), std::ios_base::out | std::ios_base::app);
		outFile << groupName;
		outFile.close();
		Session::getInstance()->updateSession(groupName);
	}else { // 실패. 해당 그룹이 없음
		cout << "해당 그룹이 없습니다." << endl;
	}
}

//그룹 리스트를 받아온다

void getGroupList() {

	DIR *dirInfo;
	struct dirent *dirEntry;
	string str1 = ".", str2 = "..";

	dirInfo = opendir("./group");

	if(dirInfo != NULL) {

		while(dirEntry = readdir(dirInfo)) {
			string entryStr(dirEntry->d_name);
			if(!(entryStr.compare(str1) == 0 || entryStr.compare(str2) == 0)) {
				cout << entryStr << endl;
			}

		}
		closedir(dirInfo);
	}

}

//그룹을 생성한다
bool makeGroup(){

	string groupName, groupLeaderName;

	groupLeaderName = Session::getInstance()->getID();

	cout << "그룹 이름 : ";
	cin >> groupName;

	string dirName = "./group/";
	string path = dirName + groupName;

	char * cPath = strdup( path.c_str());

	string configFile = path + "/config"; //그룹장을 config 파일에 저장
	if( mkdir(cPath, 0777) == 0){	// 디렉토리 생성
		ofstream outFile( configFile.c_str() );
		outFile << groupLeaderName << endl;
		outFile.close();
		cout<<"그룹 생성 완료"<<endl;
		joinGroup(groupName);
		return true;
	}
	else{
		perror("그룹 생성 실패 \n");
		if(errno = EEXIST)
			cout << "그룹이 이미 존재합니다."<<endl;
		return false;
	}
}

/*
void deleteGroup(){
//그룹 삭제
	string groupName, groupLeaderName;
	string userID;

	cout << "삭제할 그룹 이름: ";
	cin >> groupName;

	cout << "ID 입력 : ";
	cin >> userID;

	string dirName = "./group/";
	string path = dirName + groupName;
	string configFile = path + "/config";

	char * cPath = strdup( path.c_str() );

	ifstream inFile( configFile.c_str() );
	inFile >> groupLeaderName;

	if( userID == groupLeaderName){
		remove(configFile.c_str());
		if( rmdir(cPath) == 0 ){	// 디렉토리 삭제
			cout << "그룹 삭제 완료" << endl;
		}
		else{
			perror("그룹 삭제 실패 \n");
			//if( errno ==
		}
	}
	else{
		cout << "그룹장만 그룹을 삭제할 수 있습니다."<<endl;
	}
}
*/

void withdrawlGroup() { // 그룹 탈퇴

	string group = Session::getInstance()->getGroup();
	if(group.compare("") == 0) {
		cout << "가입한 그룹이 없음." << endl;
	}else {
		string ID = Session::getInstance()->getID();
		string path = "./user/" + ID;
		string pass;

		ifstream ifile(path.c_str());
		if(ifile.is_open()) {
			if(ifile.getline(inputString, MAX_SIZE)) {
				pass = inputString;
				ifile.close();
			}
		}

		ofstream outFile;
		outFile.open(path.c_str());
		outFile << pass;
		outFile << endl;
		outFile.close();

		path = "./group/" + group;

		ifstream ifile2(path.c_str());
		if(ifile2.is_open()) {
			if(ifile2.getline(inputString, MAX_SIZE)) {
				if(ID.compare(inputString) == 0) {
					char *cPath = strdup(path.c_str());

					if(rmdirs(cPath, 1) == 0) {
						cout << "그룹 삭제 완료" << endl;
					}
				}
			}
		}


		Session::getInstance()->updateSession("");
	}

}

void groupUI() { // 그룹관리 UI

	string selectMenu;

	cout << "--------------------------------" << endl;
	cout << "1. 그룹 조회" << endl;
	cout << "2. 그룹 가입" << endl;
	cout << "3. 그룹 생성" << endl;
	cout << "4. 가입그룹 조회" << endl;
	cout << "5. 그룹 탈퇴" << endl;
	cout << "*입력 선택:";
	cin >> selectMenu;


	if(selectMenu.compare("1")==0)
	{
			system("clear");
			getGroupList(); // 그룹 조회
	}
	else if(selectMenu.compare("2")==0){
			system("clear");
			joinGroup(); // 그룹 가입
	}
	else if(selectMenu.compare("3")==0){
			system("clear");
			makeGroup(); // 그룹 생성
	}
	else if(selectMenu.compare("4")==0){
			system("clear");
			cout << "가입 그룹: " << Session::getInstance()->getGroup() << endl;
	}
	else if(selectMenu.compare("5")==0){
			system("clear");
			withdrawlGroup(); // 그룹 탈퇴
	}



}

void changeSession() { // 7. 세션 변경/선택
	if(login()) { // 로그인이 성공한 경우
		cout << "세션 변경 성공" << endl;
	}else {
		cout << "세션 변경 실패" << endl;
	}
}

void MemberUI(){//1.회원탈퇴


		withdrawl();//회원탈퇴
		Session::getInstance()->destroySession();
    cout<<"회원탈퇴가 정상적으로 처리되었습니다"<<endl;
}


void printBeginMenu() {

  	string selectMenu;

  	cout << "--------------------------------" << endl;
  	cout << "1. 로그인" << endl;
  	cout << "2. 회원가입" << endl;
	cout << "3. 게스트로 로그인하기"<<endl;
    cout << "--------------------------------"<<endl;
  	cout << "*입력 선택:";
  	cin >> selectMenu;

  	if(selectMenu.compare("1")==0){
  			system("clear");
  			login();
  	}
  	else if(selectMenu.compare("2")==0){
  			system("clear");
  			signUp();
  	}
  	else if(selectMenu.compare("3")==0){
			system("clear");
			guestlogin();
  	}

  	}
//메인메뉴 출력
void printMenu() {
//	system("clear");

    cout << "--------------------------------" << endl;
	cout << "-------온라인 투표 시스템-------"<<endl;
	cout << "--------------------------------" << endl;
        if(Session::getInstance()->isAlreadyLogin()) { // 이미 로그인 중인 경우
                Session::getInstance()->printSession();
                cout << "--------------------------------" << endl;
        }


	cout << "1. 회원탈퇴" << endl;
	cout << "2. 로그아웃" << endl;
	cout << "3. 투표제안" << endl;
	cout << "4. 투표관리" << endl;
	cout << "5. 그룹관리" << endl;
	cout << "6. 현재시간 설정" << endl;
	cout << "7. Session 변경/선택" << endl;
	cout << "8. 종료" << endl;
	cout << "*입력 선택:";
}
//게스트메뉴 출력
void GuestMenu(){


	cout<<"---------------------------------"<<endl;
	cout<<"---------온라인 투표 시스템------"<<endl;
	cout<<"---------------------------------"<<endl;
	cout<<"----게스트로 로그인 하셨습니다---"<<endl;
	cout<<"---------------------------------"<<endl;

	cout<<"1. 로그아웃"<<endl;
	cout<<"2. 그룹"<<endl;
	cout<<"3. Session 변경/선택"<<endl;
	cout<<"4. 종료"<<endl;
	cout<<"*입력 선택:";


}



int main(void) {

  system("clear");

	while(true) {

		string selectMenu;
		string ID, pass;
//시작 화면 출력
    while(!Session::getInstance()->isAlreadyLogin()) {

      printBeginMenu();
    }
//게스트 로그인일경우 게스트메뉴화면 출력
	if(Session::getInstance()->getID().compare("GUEST")==0)
	{

		//게스트메뉴화면 입력
		GuestMenu();
		cin>> selectMenu;
		//게스트 로그아웃
		if(selectMenu.compare("1")==0)
		{
		system("clear");
		logout();
		}
		//게스트 그룹조회
		else if(selectMenu.compare("2")==0)
		{
		system("clear");
		groupUI();
		}
		//게스트 세션 변경
		else if(selectMenu.compare("3")==0)
		{
		system("clear");
		changeSession();
		}
		else if(selectMenu.compare("4")==0) break;


	}


	else{
		printMenu();
		cin >> selectMenu;


		if(selectMenu.compare("1")==0){
		// 회원탈퇴
				system("clear");
				MemberUI();
		}
		else if(selectMenu.compare("2")==0){ // 로그아웃
				system("clear");
				if(Session::getInstance()->isAlreadyLogin()) // 이미 로그인 중인경우
                                        logout();

		}
		else if(selectMenu.compare("3")==0){ // 투표 제안
				system("clear");
				cout << "투표제안" << endl;
				makeVote();
		}
		else if(selectMenu.compare("4")==0){ // 투표 관리
				system("clear");
				cout << "투표관리" << endl;
			//	getVoteList();
				Vote();
		}
		else if(selectMenu.compare("5")==0){ // 그룹 관리
				system("clear");
				groupUI();

		}
		else if(selectMenu.compare("6")==0){ // 현재시간
				system("clear");
		}

		else if(selectMenu.compare("7")==0){ // session 변경/선택
				system("clear");
				changeSession();
		}

		else if(selectMenu.compare("8")==0) break;


		}
	}

	return 0;
}
