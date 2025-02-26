#include <iostream>

#include "Workload.h"

using namespace std;


Workload::Workload(float TFLOPs, float paramsize, float access, float traffic){

    this->TFLOPs = TFLOPs;
    this->paramsize = paramsize;
    this->access = access;
    this-> traffic = traffic;

}


Workload::Workload(const Workload &obj){

    this->TFLOPs = obj.get_TFLOPs();
    this->paramsize = obj.get_paramsize();
    this->access = obj.get_access();
    this->traffic = obj.get_traffic();

}


Workload& Workload::operator = (const Workload &obj){

    this->TFLOPs = obj.get_TFLOPs();
    this->paramsize = obj.get_paramsize();
    this->access = obj.get_access();
    this->traffic = obj.get_traffic();
    return *this;
    
}


const float Workload::get_TFLOPs()const{

    return TFLOPs;

}


const float Workload::get_paramsize()const{

    return paramsize;

}


const float Workload::get_access()const{

    return access;

}


const float Workload::get_traffic()const{

    return traffic;

}


void Workload::set_TFLOPs(float TFLOPs){

    this->TFLOPs = TFLOPs;

}


void Workload::set_paramsize(float paramsize){

    this->paramsize = paramsize;

}


void Workload::set_access(float access){

    this->access = access;

}


void Workload::set_traffic(float traffic){

    this->traffic = traffic;

}

