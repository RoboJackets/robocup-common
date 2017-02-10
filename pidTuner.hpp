#pragma once
#include <vector>

class pidTuner {
public:

    pidTuner(float ip, float ii, float id, float Sp=1, float Si=.1, float Sd=.05);

    void start_cycle();
    void run(float err);
    bool end_cycle();

    float getP(){ return _current_pid.p; }
    float getI(){ return _current_pid.i; }
    float getD(){ return _current_pid.d; }

private:

    struct pid_set {
        pid_set(float ip=0, float ii=0, float id=0);
        float p;
        float i;
        float d;
        float score;
    };

    pid_set _initial_pid;
    pid_set _current_pid;

    int _cycles;

    float _pScale;
    float _iScale;
    float _dScale;

    std::vector <pid_set> _pid_sets;

};

