#include "pidTuner.hpp"
#include <iostream>
#include <math.h>
#include <algorithm>


PidTuner::PidTuner(float ip, float ii, float id, float Sp, float Si, float Sd) {
    _initialPid = PidSet(ip,ii,id);
    _currentPid =  _initialPid;

    _cycles = 0;
    _testNum = 0;

    _pScale = Sp;
    _iScale = Si;
    _dScale = Sd;

    _overScale = 1;

    _threshold = .5;

    _overScale = 2; //the step is divided by overscale each time it overshoots the target
}

PidTuner::PidSet::PidSet(float ip, float ii, float id) {
    p=ip;
    i=ii;
    d=id;
    score=0;
}

void PidTuner::startCycle() {
    //if old test set is done, determine new values to test
    if(_testNum==_testSets.size()) {
        _testSets.clear();
        _testSets.push_back(PidSet(_currentPid.p, _currentPid.i, _currentPid.d));
        _testSets.push_back(PidSet(_currentPid.p + _pScale, _currentPid.i, _currentPid.d));
        _testSets.push_back(PidSet(_currentPid.p, _currentPid.i + _iScale, _currentPid.d));
        _testSets.push_back(PidSet(_currentPid.p, _currentPid.i, _currentPid.d + _dScale));
        _testNum = 0;
    }
    _currentPid = _testSets[_testNum];
}

void PidTuner::run(float err) {
    _currentPid.score += fabs(err);
}

//returns if pid needs more tuning, if it does, sets up next test points
bool PidTuner::endCycle() {
    _testSets[_testNum] = _currentPid;
    _testNum += 1;

    if(_testNum==_testSets.size()) {
        //finished testing set, determine best pid, then check if we need more tests
        _cycles+=1;


        PidSet bestPid = _testSets[0];
        bestPid = *(std::min_element(std::begin(_testSets),std::end(_testSets),
            [] (PidSet const& p1, PidSet const& p2){ return p1.score < p2.score;}));


        _pScale *= _testSets[0].score > _testSets[1].score ? 1 : -1/_overScale;
        _iScale *= _testSets[0].score > _testSets[2].score ? 1 : -1/_overScale;
        _dScale *= _testSets[0].score > _testSets[3].score ? 1 : -1/_overScale;

        _currentPid = PidSet(_testSets[0].p + _pScale, _testSets[0].i + _iScale, _testSets[0].d + _dScale);

        //make sure we have a prev score, then check how much the score improved by
        if(_cycles > 1 && _prevScore - bestPid.score < _threshold) {
            _currentPid = bestPid;
            return false; //Finished tuning
        } else {
            _prevScore = bestPid.score;
            return true; //keep testing
        }
    } else {
        return true; //keep testing
    }
}
