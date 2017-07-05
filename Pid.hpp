#pragma once
#include <vector>

class Pid {
public:
    Pid(float p = 0, float i = 0, float d = 0, float dAlpha = 0)
        : kp(p), ki(i), kd(d), _integral(0), _lastError(0), _lastDeriv(0),
          _derivAlpha(dAlpha), _saturated(false)
    { }

    float run(float err, float dt);

    float kp, ki, kd;

    void set_saturated(bool is_saturated) { _saturated = is_saturated; }

private:
    Pid(Pid&);
    Pid& operator&=(Pid&);

    float _integral;

    float _lastError;

    float _lastDeriv;

    /* [0, 1] -> [all new data, all old data] */
    float _derivAlpha;

    bool _saturated;
};
