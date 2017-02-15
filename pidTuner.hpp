#pragma once
#include <vector>

class pidTuner {
public:

    pidTuner(float ip, float ii, float id, float Sp=.5, float Si=.05, float Sd=.01);

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

    pid_set _hill_vector;

    int _cycles;

    float _pScale;
    float _iScale;
    float _dScale;

    float _errThreshold;

    float _minScore;

    std::vector <pid_set> _pid_sets;

};

