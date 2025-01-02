#include <iostream>
#include <cassert>
#include "Communication.h"
using namespace std;

int main(){
    float sizes[2] = {6,10};
    float bandwidth = 10;
    float padding = 1;

    Communication NoC(sizes, bandwidth, padding);

    assert(NoC.get_size(0) == sizes[0]);
    assert(NoC.get_size(1) == sizes[1]);
    assert(NoC.get_bandwidth() == bandwidth);
    assert(NoC.get_padding() == padding);

    sizes[0] = 7;
    sizes[1] = 11;
    bandwidth = 9;
    padding = 2;

    NoC.set_size(sizes[0], 0);
    NoC.set_size(sizes[1], 1);
    NoC.set_bandwidth(bandwidth);
    NoC.set_padding(padding);

    assert(NoC.get_size(0) == sizes[0]);
    assert(NoC.get_size(1) == sizes[1]);
    assert(NoC.get_bandwidth() == bandwidth);
    assert(NoC.get_padding() == padding);

    Communication NVLink(NoC);
    assert(NVLink.get_size(0) == sizes[0]);
    assert(NVLink.get_size(1) == sizes[1]);
    assert(NVLink.get_bandwidth() == bandwidth);
    assert(NVLink.get_padding() == padding);

    sizes[0] = 1;
    sizes[1] = 3;
    bandwidth = 8;
    padding = 0.2;

    Communication Fiber(sizes, bandwidth, padding);
    Fiber = NVLink;
    assert(NVLink.get_size(0) == Fiber.get_size(0));
    assert(NVLink.get_size(1) == Fiber.get_size(1));
    assert(NVLink.get_bandwidth() == Fiber.get_bandwidth());
    assert(NVLink.get_padding() == Fiber.get_padding());


    return 0;
}