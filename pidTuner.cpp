#include "pidTuner.hpp"
#include <iostream>
#include <math.h>
#include <algorithm>

//THIS VERSION OF THE CLASS IS FOR GRAPHING THE PID SPACE ONLY
PidTuner::PidTuner(float ip, float ii, float id, float Sp, float Si, float Sd) {
    _initialPid = PidSet(ip,ii,id);
    _currentPid =  _initialPid;

    _cycles = 0;
    _testNum = 0;

    _pScale = .1; //ONLY CHECK P VALUES
    _iScale = 0;
    _dScale = 0;

    _overScale = 1;

    _threshold = .5;

    _overScale = 2; //the step is divided by overscale each time it overshoots the target

    avg = 0;

    repeat = 1;

    for(float p = 1; p<=20; p+=_pScale){
        for(int i = 0; i < repeat; i++){
            _testSets.push_back(PidSet(p,0,0));
        }
    }
    std::cout<<"PID, Error"<<std::endl;
}

PidTuner::PidSet::PidSet(float ip, float ii, float id) {
    p=ip;
    i=ii;
    d=id;
    score=0;
}

void PidTuner::startCycle() {
    _currentPid = _testSets[_testNum];
}

void PidTuner::run(float err) {
    _currentPid.score += fabs(err);
}

//returns if pid needs more tuning, if it does, sets up next test points
bool PidTuner::endCycle() {
    if(_testNum + 1 == _testSets.size()){
        std::cout<<"_______________________________________________________"<<std::endl;
        return false;
    }
    _testSets[_testNum] = _currentPid;
    _testNum += 1;

    std::cout<<_currentPid.p<<" , "<<_currentPid.score<<std::endl;

    /*if(_testNum%repeat == 0){ //avg 4 tests of the same value together
        std::cout<<_currentPid.p<<" , "<<avg/repeat<<std::endl;
        avg = 0;
    }
    else{
        avg+=_currentPid.score;
    }*/

    return true;
}
