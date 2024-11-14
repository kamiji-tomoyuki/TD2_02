// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#include "../Matrix4x4.h"
#include "../Vector3.h"

#include <cmath>

Matrix4x4 Matrix4x4::operator-() const
{
    Matrix4x4 result{};

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = -m[i][j];

    return result;
}

void Matrix4x4::SetIdentity()
{
    for (int i = 0; i < 4; i++)
        m[i][i] = 1;
    return;
}

Matrix4x4 Matrix4x4::Identity()
{
    Matrix4x4 result{};

    for (int i = 0; i < 4; i++)
        result.m[i][i] = 1;

    return result;
}

Matrix4x4 Matrix4x4::RotateXMatrix(float _radian)
{
    Matrix4x4 result{};

    result.m[0][0] = 1.0f;
    result.m[1][1] = std::cosf(_radian);
    result.m[1][2] = std::sinf(_radian);
    result.m[2][1] = -std::sinf(_radian);
    result.m[2][2] = std::cosf(_radian);
    result.m[3][3] = 1.0f;

    return result;
}

Matrix4x4 Matrix4x4::RotateYMatrix(float _radian)
{
    Matrix4x4 result{};

    result.m[1][1] = 1.0f;
    result.m[3][3] = 1.0f;
    result.m[0][0] = std::cosf(_radian);
    result.m[0][2] = -std::sinf(_radian);
    result.m[2][0] = std::sinf(_radian);
    result.m[2][2] = std::cosf(_radian);

    return result;
}

Matrix4x4 Matrix4x4::RotateZMatrix(float _radian)
{
    Matrix4x4 result{};

    for (int i = 3; i > 1; --i)
    {
        result.m[i][i] = 1.0f;
    }
    result.m[0][0] = std::cos(_radian);
    result.m[1][0] = -std::sin(_radian);
    result.m[0][1] = std::sin(_radian);
    result.m[1][1] = std::cos(_radian);

    return result;
}

Matrix4x4 Matrix4x4::AffineMatrix(const Vector3& _scale, const Vector3& _rotate, const Vector3& _translate)
{
    Matrix4x4 result{};
    Matrix4x4 rotateMatrix = RotateXMatrix(_rotate.x) * (RotateYMatrix(_rotate.y) * RotateZMatrix(_rotate.z));

    for (int i = 0; i < 3; i++)
    {
        result.m[0][i] = _scale.x * rotateMatrix.m[0][i];
        result.m[1][i] = _scale.y * rotateMatrix.m[1][i];
        result.m[2][i] = _scale.z * rotateMatrix.m[2][i];
    }
    result.m[3][0] = _translate.x;
    result.m[3][1] = _translate.y;
    result.m[3][2] = _translate.z;

    result.m[3][3] = 1.0f;

    return result;
}

Matrix4x4 Matrix4x4::PerspectiveFovMatrix(float _fovY, float _aspectRatio, float _nearClip, float _farClip)
{
    Matrix4x4 result{};

    float _h = 1.0f / std::tan(_fovY / 2.0f);
    float _w = _h / _aspectRatio;

    result.m[0][0] = _w;
    result.m[1][1] = _h;
    result.m[2][2] = _farClip / (_farClip - _nearClip);
    result.m[2][3] = 1.0f;
    result.m[3][2] = -_nearClip * _farClip / (_farClip - _nearClip);
    result.m[3][3] = 0.0f;

    return result;
}

Matrix4x4 Matrix4x4::OrthographicMatrix(float _left, float _top, float _right, float _bottom, float _nearClip, float _farClip)
{
    Matrix4x4 result{};

    result.m[0][0] = 2.0f / (_right - _left);
    result.m[1][1] = 2.0f / (_top - _bottom);
    result.m[2][2] = 1.0f / (_farClip - _nearClip);
    result.m[3][0] = -(_right + _left) / (_right - _left);
    result.m[3][1] = -(_top + _bottom) / (_top - _bottom);
    result.m[3][2] = -_nearClip / (_farClip - _nearClip);
    result.m[3][3] = 1.0f;

    return result;
}

Matrix4x4 Matrix4x4::ViewportMatrix(float _left, float _top, float _width, float _height, float _minDepth, float _maxDepth)
{
    Matrix4x4 result{};

    result.m[0][0] = _width / 2.0f;
    result.m[1][1] = -_height / 2.0f;
    result.m[2][2] = _maxDepth - _minDepth;
    result.m[3][0] = _left + _width / 2.0f;
    result.m[3][1] = _top + _height / 2.0f;
    result.m[3][2] = _minDepth;

    return result;
}

Matrix4x4 Matrix4x4::ScaleMatrix(const Vector3& _scale)
{
    Matrix4x4 result{};

    for (int i = 0; i < 4; i++)
    {
        if (i == 3)
        {
            result.m[i][i] = 1.0f;
            break;
        }
        result.m[i][i] = *(&_scale.x + i);
    }

    return result;
}

Matrix4x4 Matrix4x4::TranslateMatrix(const Vector3& _translate)
{
    Matrix4x4 result{};

    for (int i = 0; i < 4; i++) result.m[i][i] = 1.0f;

    result.m[3][0] += _translate.x;
    result.m[3][1] += _translate.y;
    result.m[3][2] += _translate.z;

    return result;
}

Matrix4x4 Matrix4x4::Inverse() const
{
    Matrix4x4 result{};

#pragma region 行列式の計算
    float _a_ =
        m[0][0] * m[1][1] * m[2][2] * m[3][3] +
        m[0][0] * m[1][2] * m[2][3] * m[3][1] +
        m[0][0] * m[1][3] * m[2][1] * m[3][2] -
        m[0][0] * m[1][3] * m[2][2] * m[3][1] -
        m[0][0] * m[1][2] * m[2][1] * m[3][3] -
        m[0][0] * m[1][1] * m[2][3] * m[3][2] -
        m[0][1] * m[1][0] * m[2][2] * m[3][3] -
        m[0][2] * m[1][0] * m[2][3] * m[3][1] -
        m[0][3] * m[1][0] * m[2][1] * m[3][2] +
        m[0][3] * m[1][0] * m[2][2] * m[3][1] +
        m[0][2] * m[1][0] * m[2][1] * m[3][3] +
        m[0][1] * m[1][0] * m[2][3] * m[3][2] +
        m[0][1] * m[1][2] * m[2][0] * m[3][3] +
        m[0][2] * m[1][3] * m[2][0] * m[3][1] +
        m[0][3] * m[1][1] * m[2][0] * m[3][2] -
        m[0][3] * m[1][2] * m[2][0] * m[3][1] -
        m[0][2] * m[1][1] * m[2][0] * m[3][3] -
        m[0][1] * m[1][3] * m[2][0] * m[3][2] -
        m[0][1] * m[1][2] * m[2][3] * m[3][0] -
        m[0][2] * m[1][3] * m[2][1] * m[3][0] -
        m[0][3] * m[1][1] * m[2][2] * m[3][0] +
        m[0][3] * m[1][2] * m[2][1] * m[3][0] +
        m[0][2] * m[1][1] * m[2][3] * m[3][0] +
        m[0][1] * m[1][3] * m[2][2] * m[3][0];
#pragma endregion

#pragma region 各成分の計算
    // 一行目
    result.m[0][0] =
        m[1][1] * m[2][2] * m[3][3] +
        m[1][2] * m[2][3] * m[3][1] +
        m[1][3] * m[2][1] * m[3][2] -
        m[1][3] * m[2][2] * m[3][1] -
        m[1][2] * m[2][1] * m[3][3] -
        m[1][1] * m[2][3] * m[3][2];
    result.m[0][1] =
        -m[0][1] * m[2][2] * m[3][3] -
        m[0][2] * m[2][3] * m[3][1] -
        m[0][3] * m[2][1] * m[3][2] +
        m[0][3] * m[2][2] * m[3][1] +
        m[0][2] * m[2][1] * m[3][3] +
        m[0][1] * m[2][3] * m[3][2];
    result.m[0][2] =
        m[0][1] * m[1][2] * m[3][3] +
        m[0][2] * m[1][3] * m[3][1] +
        m[0][3] * m[1][1] * m[3][2] -
        m[0][3] * m[1][2] * m[3][1] -
        m[0][2] * m[1][1] * m[3][3] -
        m[0][1] * m[1][3] * m[3][2];
    result.m[0][3] =
        -m[0][1] * m[1][2] * m[2][3] -
        m[0][2] * m[1][3] * m[2][1] -
        m[0][3] * m[1][1] * m[2][2] +
        m[0][3] * m[1][2] * m[2][1] +
        m[0][2] * m[1][1] * m[2][3] +
        m[0][1] * m[1][3] * m[2][2];
    // 2行目
    result.m[1][0] =
        -m[1][0] * m[2][2] * m[3][3] -
        m[1][2] * m[2][3] * m[3][0] -
        m[1][3] * m[2][0] * m[3][2] +
        m[1][3] * m[2][2] * m[3][0] +
        m[1][2] * m[2][0] * m[3][3] +
        m[1][0] * m[2][3] * m[3][2];
    result.m[1][1] =
        m[0][0] * m[2][2] * m[3][3] +
        m[0][2] * m[2][3] * m[3][0] +
        m[0][3] * m[2][0] * m[3][2] -
        m[0][3] * m[2][2] * m[3][0] -
        m[0][2] * m[2][0] * m[3][3] -
        m[0][0] * m[2][3] * m[3][2];
    result.m[1][2] =
        -m[0][0] * m[1][2] * m[3][3] -
        m[0][2] * m[1][3] * m[3][0] -
        m[0][3] * m[1][0] * m[3][2] +
        m[0][3] * m[1][2] * m[3][0] +
        m[0][2] * m[1][0] * m[3][3] +
        m[0][0] * m[1][3] * m[3][2];
    result.m[1][3] =
        m[0][0] * m[1][2] * m[2][3] +
        m[0][2] * m[1][3] * m[2][0] +
        m[0][3] * m[1][0] * m[2][2] -
        m[0][3] * m[1][2] * m[2][0] -
        m[0][2] * m[1][0] * m[2][3] -
        m[0][0] * m[1][3] * m[2][2];
    // 3行目
    result.m[2][0] =
        m[1][0] * m[2][1] * m[3][3] +
        m[1][1] * m[2][3] * m[3][0] +
        m[1][3] * m[2][0] * m[3][1] -
        m[1][3] * m[2][1] * m[3][0] -
        m[1][1] * m[2][0] * m[3][3] -
        m[1][0] * m[2][3] * m[3][1];
    result.m[2][1] =
        -m[0][0] * m[2][1] * m[3][3] -
        m[0][1] * m[2][3] * m[3][0] -
        m[0][3] * m[2][0] * m[3][1] +
        m[0][3] * m[2][1] * m[3][0] +
        m[0][1] * m[2][0] * m[3][3] +
        m[0][0] * m[2][3] * m[3][1];
    result.m[2][2] =
        m[0][0] * m[1][1] * m[3][3] +
        m[0][1] * m[1][3] * m[3][0] +
        m[0][3] * m[1][0] * m[3][1] -
        m[0][3] * m[1][1] * m[3][0] -
        m[0][1] * m[1][0] * m[3][3] -
        m[0][0] * m[1][3] * m[3][1];
    result.m[2][3] =
        -m[0][0] * m[1][1] * m[2][3] -
        m[0][1] * m[1][3] * m[2][0] -
        m[0][3] * m[1][0] * m[2][1] +
        m[0][3] * m[1][1] * m[2][0] +
        m[0][1] * m[1][0] * m[2][3] +
        m[0][0] * m[1][3] * m[2][1];
    // 4行目
    result.m[3][0] =
        -m[1][0] * m[2][1] * m[3][2] -
        m[1][1] * m[2][2] * m[3][0] -
        m[1][2] * m[2][0] * m[3][1] +
        m[1][2] * m[2][1] * m[3][0] +
        m[1][1] * m[2][0] * m[3][2] +
        m[1][0] * m[2][2] * m[3][1];
    result.m[3][1] =
        m[0][0] * m[2][1] * m[3][2] +
        m[0][1] * m[2][2] * m[3][0] +
        m[0][2] * m[2][0] * m[3][1] -
        m[0][2] * m[2][1] * m[3][0] -
        m[0][1] * m[2][0] * m[3][2] -
        m[0][0] * m[2][2] * m[3][1];
    result.m[3][2] =
        -m[0][0] * m[1][1] * m[3][2] -
        m[0][1] * m[1][2] * m[3][0] -
        m[0][2] * m[1][0] * m[3][1] +
        m[0][2] * m[1][1] * m[3][0] +
        m[0][1] * m[1][0] * m[3][2] +
        m[0][0] * m[1][2] * m[3][1];
    result.m[3][3] =
        m[0][0] * m[1][1] * m[2][2] +
        m[0][1] * m[1][2] * m[2][0] +
        m[0][2] * m[1][0] * m[2][1] -
        m[0][2] * m[1][1] * m[2][0] -
        m[0][1] * m[1][0] * m[2][2] -
        m[0][0] * m[1][2] * m[2][1];

#pragma endregion

#pragma region 各成分 / 行列式
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] /= _a_;
        }
#pragma endregion

    return result;
}

Matrix4x4 Matrix4x4::Transpose() const
{
    Matrix4x4 result{};

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = m[j][i];

    return result;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& _rm)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] += _rm.m[i][j];
        }
    }
    return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& _rm)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] -= _rm.m[i][j];
        }
    }
    return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& _rm)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] *= _rm.m[i][j];
        }
    }
    return *this;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& _rm) const
{
    Matrix4x4 result{};

    for (int off = 0; off < 4; off++)
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result.m[off][i] += m[off][j] * _rm.m[j][i];

    return result;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& _rm) const
{
    Matrix4x4 result{};

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = m[i][j] + _rm.m[i][j];

    return result;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& _rm) const
{
    Matrix4x4 result{};

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = m[i][j] - _rm.m[i][j];

    return result;
}
