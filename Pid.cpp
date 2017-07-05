#include "Pid.hpp"

#include <cmath>

using namespace std;

float Pid::run(const float err) {
    if (isnan(err)) {
        return 0;
    }

    float newDErr = err - _lastErr;
    float dErr = _dAlpha * _lastDErr + (1 - _dAlpha) * newDErr;
    _lastDErr = newDErr;

    _lastErr = err;

    if (!_saturated)
        _errSum += err;

    return (err * kp) + (_errSum * ki) + (dErr * kd);
}
