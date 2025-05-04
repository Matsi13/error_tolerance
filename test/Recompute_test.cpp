#include <iostream>

#include "Macro.h"
#include "Workload.h"
#include "Recompute.h"

using namespace std;

int main(){

    Workload workload(1, 3, 5, 7);
    float input_size = 2;
    float distance2end = 10;
    bool is_checkpoint = true;
    

    Workload result = recompute_memory(workload, input_size, distance2end, is_checkpoint);

    cout << result.get_TFLOPs() << endl;

    return 0;

}
