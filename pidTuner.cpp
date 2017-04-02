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

    _pScale = .5; //ONLY CHECK P VALUES
    _iScale = 0;
    _dScale = 0;

    _overScale = 1;

    _threshold = .5;

    _overScale = 2; //the step is divided by overscale each time it overshoots the target

    avg = 0;

    for(float p = -10; p<=10.5; p+=_pScale){
        _testSets.push_back(PidSet(p,ii,id));
        _testSets.push_back(PidSet(p,ii,id));
        _testSets.push_back(PidSet(p,ii,id));
        _testSets.push_back(PidSet(p,ii,id));
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

    if(_testNum%4 == 0){ //avg 4 tests of the same value together
        std::cout<<_testNum<<","<<avg/4.0<<std::endl;
        avg = 0;
    }
    else{
        avg+=_currentPid.score;
    }
    return true;
}
