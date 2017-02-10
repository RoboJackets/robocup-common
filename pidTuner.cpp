#include "pidTuner.hpp"
#include <iostream>
#include <math.h>

pidTuner::pidTuner(float ip, float ii, float id, float Sp, float Si, float Sd){
    _initial_pid = pid_set(ip,ii,id);
    _cycles = 0;

    _pScale = Sp;
    _iScale = Si;
    _dScale = Sd;
}

pidTuner::pid_set::pid_set(float ip, float ii, float id){
    p=ip;
    i=ii;
    d=id;
    score=0;
}

void pidTuner::start_cycle(){
    std::cout<<"C++: Start Cycle"<<std::endl;

    pid_set _pid = _initial_pid;

    //Initial Test Points,Tests current constants then produces cube centered on initial PID with sidelength of 2*_scale
    if(1 <= _cycles && _cycles <= 8){
        _cycles--;
        if(_cycles>3)
            _pid.p += _pScale;
        else
            _pid.p -= _pScale;
        if(_cycles%4 >= 2)
            _pid.d += _iScale;
        else
            _pid.d -= _iScale;
        if(_cycles%2)
            _pid.i += _dScale;
        else
            _pid.i -= _dScale;
        _cycles++;
    }
    std::cout<<"C++ CYCLE: "<<_cycles<<" : "<<_pid.p<<" , "<<_pid.i<<" , "<<_pid.d<<std::endl;
    _current_pid = _pid;
}

void pidTuner::run(float err){
    _current_pid.score+=fabs(err);
}

//cleans up and returns if pid needs more tuning
bool pidTuner::end_cycle(){
    std::cout<<"C++: End CYCLE: "<<_current_pid.score<<std::endl;
    _pid_sets.push_back(_current_pid);
    _cycles+=1;
    return true;
}
