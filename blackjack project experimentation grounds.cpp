#include <iostream>
#include <cmath>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
using namespace std;

int main (){
    srand (time(0));
    for (int oldValueIndex = 0; oldValueIndex < 52; oldValueIndex++){
        int newValueIndex = rand()%52;
        cout << newValueIndex << " ";
    }
}
