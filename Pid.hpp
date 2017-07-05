#pragma once
#include <vector>

class Pid {
public:
    Pid(float p = 0, float i = 0, float d = 0, float dAlpha = 0)
        : kp(p), ki(i), kd(d), _errSum(0), _lastErr(0), _lastDErr(0),
          _dAlpha(dAlpha), _saturated(false)
    { }

    /** Runs the pid loop and returns the error */
    float run(const float err);

    float kp, ki, kd;

    void set_saturated(bool is_saturated) { _saturated = is_saturated; }

private:
    Pid(Pid&);
    Pid& operator&=(Pid&);

    float _errSum;

    float _lastErr;

    float _lastDErr;

    /* Low pass on derivative, 0 means all new data, 1 means all old data */
    float _dAlpha;

    bool _saturated;
};
