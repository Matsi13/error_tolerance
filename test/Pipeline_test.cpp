#include <cassert>
#include <iostream>
#include <stack>

#include "Macro.h"
#include "Workload.h"
#include "Pipeline.h"

using namespace std;

int main(){

    Workload workload1(1, 3, 5, 7);
    Workload workload2(2, 4, 6, 8);
    Workload workload3(3, 6, 9, 12);

    stack<Workload> workloads;
    workloads.push(workload1);
    workloads.push(workload2);
    workloads.push(workload3);

    Workload result = pipeline_merge(workloads);

    cout << result.get_TFLOPs() << endl;

    return 0;

}
