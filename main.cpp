#include <iostream>
using namespace std;

// 메인 화면을 그리기 위한 클래스 생성
class StartMenu {
private:
	void DrawStartMenu() {
		cout << "이 부분에 메인 화면을 구성합니다.\n";
		getchar();
	}
public:
	void InitStartMenu() {
		StartMenu::DrawStartMenu();
	}
};

int main(void)
{
	StartMenu MenuControl;
	MenuControl.InitStartMenu();

	system("cls");	
	return 0;
}