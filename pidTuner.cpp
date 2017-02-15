#include "pidTuner.hpp"
#include <iostream>
#include <math.h>

pidTuner::pidTuner(float ip, float ii, float id, float Sp, float Si, float Sd){
    _initial_pid = pid_set(ip,ii,id);
    _cycles = 0;

    _pScale = Sp;
    _iScale = Si;
    _dScale = Sd;

    _errThreshold = .1;
}

pidTuner::pid_set::pid_set(float ip, float ii, float id){
    p=ip;
    i=ii;
    d=id;
    score=0;
}

void pidTuner::start_cycle(){
    //std::cout<<"C++: Start Cycle"<<std::endl;

    pid_set _pid = pid_set(_initial_pid.p,_initial_pid.i,_initial_pid.d);

    std::cout<<_cycles<<std::endl;
    switch(_cycles){
        case 0:
            std::cout<<"0"<<std::endl;
            break;
        case 1:
            _pid.p += _pScale;
            std::cout<<"p"<<std::endl;
            break;
        case 2:
            _pid.i += _iScale;
            std::cout<<"i"<<std::endl;
            break;
        case 3:
            _pid.d += _dScale;
            std::cout<<"d"<<std::endl;
            break;
        default:
            std::cout<<"default"<<std::endl;
            _pid = pid_set(_current_pid.p,_current_pid.i,_current_pid.d);
            _pid.p += _hill_vector.p * _pScale;
            _pid.i += _hill_vector.i * _iScale;
            _pid.d += _hill_vector.d * _dScale;
    }

    std::cout<<"pid: "<<_pid.p<<" , "<<_pid.i<<" , "<<_pid.d<<std::endl;
    _current_pid = _pid;
}

void pidTuner::run(float err){
    _current_pid.score+=fabs(err);
}

//cleans up and returns if pid needs more tuning
bool pidTuner::end_cycle(){
    //std::cout<<"C++: End CYCLE: "<<_current_pid.score<<std::endl;
    _pid_sets.push_back(_current_pid);
    _cycles+=1;
    std::cout<<"end : "<<_cycles<<" : "<<_current_pid.score<<std::endl;

    if(_current_pid.score > _errThreshold){
        if(_cycles==1){
            _initial_pid.score = _current_pid.score;
            _minScore = _current_pid.score;
        }
        else{
            if(_current_pid.score < _minScore){
                _minScore = _current_pid.score;
            }
            if(_cycles==4){
                float vp = _initial_pid.score > _pid_sets[1].score ? 1 : -1;
                float vi = _initial_pid.score > _pid_sets[2].score ? 1 : -1;
                float vd = _initial_pid.score > _pid_sets[3].score ? 1 : -1;
                _hill_vector = pid_set(vp,vi,vd);
                _current_pid = _initial_pid;

                for(int i = 0; i<4; i++){
                    std::cout<<i<<": "<<_pid_sets[i].p<<" , "<<_pid_sets[i].i<<" , "<<_pid_sets[i].d<<" : "<<_pid_sets[i].score<<std::endl;
                }
                std::cout<<"HILL: "<<vp<<" , "<<vi<<" , "<<vd<<std::endl;
            }
        }

        return true;
    }
    return false;
}
