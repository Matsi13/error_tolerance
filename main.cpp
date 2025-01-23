#include <iostream>
#include <string>

#include "Memory.h"
#include "Macro.h"

using namespace std;

int main(){

    string left = {MEMORY_UNIT, COMMUNICATION_UNIT, COMMUNICATION_UNIT, MEMORY_UNIT};
    cout << left << endl;
    cout << left.size() << endl;
    return 0;
}

