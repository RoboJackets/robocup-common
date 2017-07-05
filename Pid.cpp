#include "Pid.hpp"

#include <cmath>

using namespace std;

Pid::Pid(float p, float i, float d, unsigned int windup) : _oldErr() {
    _errSum = 0;

    _lastErr = 0;

    kp = p;
    ki = i;
    kd = d;

    _saturated = false;
}

float Pid::run(const float err) {
    if (isnan(err)) {
        return 0;
    }
    float dErr = err - _lastErr;

    _lastErr = err;

    if (!_saturated)
        _errSum += err;

    return (err * kp) + (_errSum * ki) + (dErr * kd);
}
