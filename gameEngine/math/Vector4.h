// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#pragma once

#include "Vector3.h"
#include "Vector2.h"
#include "Color.h"

/// <summary>
/// 4th Dimension Vector
/// </summary>
class Vector4 final {
public:
    float x;
    float y;
    float z;
    float w;

    inline Vector4() : x(), y(), z(), w() {};

    inline Vector4(const float& _x, const float& _y, const float& _z, const float& _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
        return;
    }

    inline Vector4(const float& _x, const float& _y, const float& _z)
    {
        x = _x;
        y = _y;
        z = _z;
        w = 0.0f;
        return;
    }

    inline Vector4(const float& _x, const float& _y)
    {
        x = _x;
        y = _y;
        z = 0.0f;
        w = 0.0f;
        return;
    }

    inline Vector4(const float& _x)
    {
        x = _x;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
        return;
    }

    inline Vector4(const Vector3& _vec3)
    {
        x = _vec3.x;
        y = _vec3.y;
        z = _vec3.z;
        w = 0.0f;
        return;
    }

    inline Vector4(const Vector2& _vec2)
    {
        x = _vec2.x;
        y = _vec2.y;
        z = 0.0f;
        w = 0.0f;
        return;
    }

    inline Vector4(const Color& _color)
    {
        *this = _color.Vec4();
        return;
    }

    inline Vector3 xyz() { return Vector3(x, y, z); }

    inline Vector2 xy() { return Vector2(x, y); }

    /// マイナス符号
    Vector4 operator-() const;

    /// =======
    /// Vector4
    /// =======

    Vector4& operator+=(const Vector4& _rv);
    Vector4& operator-=(const Vector4& _rv);
    Vector4& operator*=(const Vector4& _rv);

    Vector4 operator+(const Vector4& _v);
    Vector4 operator-(const Vector4& _v);
    Vector4 operator*(const Vector4& _v);

    /// =====
    /// float
    /// =====

    Vector4 operator+(float) = delete;
    Vector4 operator-(float) = delete;
    Vector4 operator*(float _f);
    Vector4 operator/(float _f);
    Vector4& operator+=(float) = delete;
    Vector4& operator-=(float) = delete;
    Vector4& operator*=(float _f);
    Vector4& operator/=(float _f);

    /// =======
    /// Vector2
    /// =======

    Vector4& operator+=(const Vector2& _rv);
    Vector4& operator-=(const Vector2& _rv);
    Vector4& operator*=(const Vector2& _rv);

    Vector4 operator+(const Vector2& _v);
    Vector4 operator-(const Vector2& _v);
    Vector4 operator*(const Vector2& _v);

    /// =======
    /// Vector3
    /// =======

    Vector4& operator+=(const Vector3& _rv);
    Vector4& operator-=(const Vector3& _rv);
    Vector4& operator*=(const Vector3& _rv);

    Vector4 operator+(const Vector3& _v);
    Vector4 operator-(const Vector3& _v);
    Vector4 operator*(const Vector3& _v);

};