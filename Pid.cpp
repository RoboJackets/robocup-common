#include "Pid.hpp"

#include <cmath>

using namespace std;

Pid::Pid(float p, float i, float d, float dAlpha, unsigned int windup)
        : kp(p), ki(i), kd(d), derivAlpha(dAlpha), _windup(0), _windupLoc(0), _errSum(0), _lastError(0),
          _lastDeriv(0), _oldErr(), _saturated(false) {
    setWindup(windup);
}

void Pid::setWindup(unsigned int w) {
    if (w != _windup) {
        _windup = w;

        if (w > 0) {
            _oldErr.resize(w);
            _errSum = 0;
            std::fill(_oldErr.begin(), _oldErr.end(), 0);
        } else {
            _oldErr.clear();
        }
    }
}

float Pid::run(const float err, float dt) {
    if (isnan(err) || isnan(dt) || dt == 0) return 0;

    float integralErr;
    // integral
    if (_saturated) {
        integralErr = err *dt;
    } else {
        integralErr = 0;
    }

    _errSum += err;

    if (_windup > 0) {
        _errSum -= _oldErr[_windupLoc];
        _oldErr[_windupLoc] = integralErr;

        _windupLoc = (_windupLoc + 1) % _windup;
    }

    // derivative (with alpha filter)
    float newDeriv = (err - _lastError) / dt; // compute newest derivative
    float derivative = derivAlpha * _lastDeriv + (1 - derivAlpha) * newDeriv;

    // update our state variables
    _lastError = err;
    _lastDeriv = derivative;

    return (err * kp) + (_errSum * ki) + (derivative * kd);
}

void Pid::clearWindup() {
    _errSum = 0;
    std::fill(_oldErr.begin(), _oldErr.end(), 0);
}