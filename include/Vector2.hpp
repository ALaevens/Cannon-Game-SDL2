#pragma once

#include <cmath>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>

template <typename T>
struct Vector2 {
    T x = 0;
    T y = 0;

    Vector2() = default;
    Vector2(T p_x, T p_y) : x(p_x), y(p_y) {};

    void fromMagAngle(float magnitude, float radians) {
        x = magnitude * cos(radians);
        y = magnitude * sin(radians);
    }

    std::pair<float, float> toMagAngle() {
        std::pair<float, float> res;

        res.first = sqrt(pow(x, 2) + pow(y, 2));

        if (x == 0 && y == 0)
            res.second = 0;
        else
            res.second = atan2(y, x);
        
        return res;
    }

    Vector2 operator + (Vector2 const &obj) {
        Vector2 res;
        res.x = x + obj.x;
        res.y = y + obj.y;
        return res;
    }

    Vector2 operator - (Vector2 const &obj) {
        Vector2 res;
        res.x = x - obj.x;
        res.y = y - obj.y;
        return res;
    }

    Vector2 operator * (Vector2 const &obj) {
        Vector2 res;
        res.x = x * obj.x;
        res.y = y * obj.y;
        return res;
    }

    Vector2 operator * (const float scalar) {
        Vector2 res;
        res.x = x * scalar;
        res.y = y * scalar;
        return res;
    }

    Vector2 operator / (Vector2 const &obj) {
        Vector2 res;
        res.x = x / obj.x;
        res.y = y / obj.y;
        return res;
    }

    Vector2 exponent(float exp) {
        Vector2 res;
        res.x = pow(x, exp);
        res.y = pow(y, exp);
        return res;
    }

    std::string str() {
        std::stringstream ss;
        ss << "X: " << x << ", Y: " << y;
        return ss.str();
    }
};