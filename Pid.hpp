#pragma once
#include <vector>
#include "pidTuner.hpp"

class Pid {
public:

    Pid(float p = 0, float i = 0, float d = 0, unsigned int windup = 0);

    unsigned int windup() const { return _windup; }

    /** Runs the pid loop and returns the error */
    float run(const float err);

    void setWindup(unsigned int w);

    /** clear any windup */
    void clearWindup();


    //tuner stuff
    void initialize_tuner();

    void set_from_tuner();

    void start_cycle();
    void run();
    bool end_cycle();


    float kp, ki, kd;

private:

    /** amount to sum up */
    unsigned int _windup;

    unsigned int _windupLoc;

    float _errSum;

    float _lastErr;

    std::vector<float> _oldErr;

    pidTuner* _tuner;
};
