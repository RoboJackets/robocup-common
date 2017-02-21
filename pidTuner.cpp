#include "pidTuner.hpp"
#include <iostream>
#include <math.h>


pidTuner::pidTuner(float ip, float ii, float id, float Sp, float Si, float Sd){
    _initial_pid = pid_set(ip,ii,id);
    _current_pid =  _initial_pid;
    _hill_vector = pid_set(1,1,1);

    _cycles = 0;
    _test_num = 0;

    _pScale = Sp;
    _iScale = Si;
    _dScale = Sd;
    _overScale = 1;

    _errThreshold = .1;
}

pidTuner::pid_set::pid_set(float ip, float ii, float id){
    p=ip;
    i=ii;
    d=id;
    score=0;
}

void pidTuner::start_cycle(){
    //determine values to test
    if(_test_num==_test_sets.size()){
         _test_sets.clear();

        for(int i=0;i<4;i++){
            pid_set _pid = pid_set(_current_pid.p,_current_pid.i,_current_pid.d);

            switch(i){
                case 0:
                    break;
                case 1:
                    _pid.p += _pScale;
                    break;
                case 2:
                    _pid.i += _iScale;
                    break;
                case 3:
                    _pid.d += _dScale;
                    break;
                }
                std::cout<<_pid.p<<" "<<_pid.i<<" "<<_pid.d<<std::endl;
                _test_sets.push_back(_pid);
        }

        _test_num = 0;
        if(_cycles != 0){
            //we have data on the initial value already
            _test_sets[0] = _current_pid;
            _test_num++;
        }
    }
    _current_pid = _test_sets[_test_num];
}

void pidTuner::run(float err){
    _current_pid.score += fabs(err);
}

//returns if pid needs more tuning, if it does, sets up next test points
bool pidTuner::end_cycle(){
    //score old value
    _test_sets[_test_num] = _current_pid;
    std::cout<<"test set #"<<_test_num<<" set to: "<<_current_pid.score<<std::endl;

    _test_num += 1;

    if(_test_num==_test_sets.size()){
        //finished testing set, determine best pid, then check if we need more tests
        _cycles+=1;


        pid_set _best_pid = _test_sets[0];
        for(int i=0; i<_test_sets.size();i++){
            if(_test_sets[i].score<_current_pid.score){
                _best_pid = _test_sets[i];
            }
        }

        std::cout<<"best pid: "<<_best_pid.p<<" "<<_best_pid.i<<" "<<_best_pid.d<<" : "<<_best_pid.score<<std::endl;

        if(_best_pid == _test_sets[0]){
            std::cout<<"Reverse"<<std::endl;
            _overScale = 2;
        }
        else{
            _overScale = 1;
        }

        //this may not be helpful
        _pScale *= _test_sets[0].score > _test_sets[1].score ? 1 : -1/_overScale;
        _iScale *= _test_sets[0].score > _test_sets[2].score ? 1 : -1/_overScale;
        _dScale *= _test_sets[0].score > _test_sets[3].score ? 1 : -1/_overScale;

        _current_pid.p = _test_sets[0].p + _pScale;
        _current_pid.i = _test_sets[0].i + _iScale;
        _current_pid.d = _test_sets[0].d + _dScale;

        std::cout<<"HILL: "<<_pScale<<" "<<_iScale<<" "<<_dScale<<std::endl;

        return true;
    }
    else{
        //keep testing
        return true;
    }
}
