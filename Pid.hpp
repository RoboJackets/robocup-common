#pragma once
#include <vector>

class Pid {
public:
    Pid(float p = 0, float i = 0, float d = 0, unsigned int windup = 0);

    /** Runs the pid loop and returns the error */
    float run(const float err);

    float kp, ki, kd;

    void set_saturated(bool is_saturated) { _saturated = is_saturated; }

private:
    Pid(Pid&);
    Pid& operator&=(Pid&);

    bool _saturated;
    float _errSum;

    float _lastErr;

    std::vector<float> _oldErr;
};
