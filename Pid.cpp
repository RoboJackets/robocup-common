#include "Pid.hpp"

#include <cmath>

using namespace std;

float Pid::run(float err, float dt) {
    if (isnan(err) || isnan(dt) || dt == 0) return 0;

    // integral
    if (!_saturated) _integral += err * dt;

    // derivative (with alpha filter)
    float newDeriv = (err - _lastError) / dt; // compute newest derivative
    float derivative = _derivAlpha * _lastDeriv + (1 - _derivAlpha) * newDeriv;

    // update our state variables
    _lastError = _lastError;
    _lastDeriv = newDeriv;

    return (err * kp) + (_integral * ki) + (derivative * kd);
}
