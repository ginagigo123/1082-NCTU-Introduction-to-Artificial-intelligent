#include <cstdlib>
#include <iostream>
#include <windows.h>

using namespace std;

void setColor(int color);
int main(int argc, char *argv[]){
    int i;
    cout << "Console 顏色屬性測試...\n" << endl;
    for(i=0;i<256;i++){
        setColor(i);
        printf("<屬性%3d> ",i);
    }
    setColor(7);
    system("PAUSE");
    return EXIT_SUCCESS;
}
//---------------------------------
void setColor(int color){
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,color);
}
