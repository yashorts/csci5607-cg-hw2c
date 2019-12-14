#ifndef MAT4_HPP
#define MAT4_HPP

#include <cmath>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <algorithm>
#include <utility>
#include "trimesh.hpp"
#include "vec3.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

enum INIT {
    IDENTITY, ONES, ZEROS
};

class Mat4 {
    // row major order
    std::vector<float> values;
    const int nR, nC;

    friend std::ostream &operator<<(std::ostream &, const Mat4 &);

public:

    // Default is identity transform, can be initialized to ONES, ZEROS if needed
    explicit Mat4(enum INIT init = IDENTITY) : nR(4), nC(4) {
        values.resize(nR * nC);
        switch (init) {
            case IDENTITY:
                setIdentity();
                break;
            case ONES:
                setOnes();
                break;
            case ZEROS:
                setZeros();
                break;
            default:
                setIdentity();
                break;
        }
    }

    // Rotation about z-axis transform
    explicit Mat4(const float angleInDegrees) : nR(4), nC(4) {
        values.resize(nR * nC);
        setIdentity();
        values[0] = cos(angleInDegrees / 180 * M_PI);
        values[1] = -sin(angleInDegrees / 180 * M_PI);
        values[4] = sin(angleInDegrees / 180 * M_PI);
        values[5] = cos(angleInDegrees / 180 * M_PI);
    }

    // Translation transform
    explicit Mat4(const Vec3 translation) : nR(4), nC(4) {
        values.resize(nR * nC);
        setIdentity();
        values[3] = translation.x;
        values[7] = translation.y;
        values[11] = translation.z;
    }

    // Scale transform
    explicit Mat4(const float scaleX, const float scaleY, const float scaleZ) : nR(4), nC(4) {
        values.resize(nR * nC);
        setIdentity();
        values[0] = scaleX;
        values[5] = scaleY;
        values[10] = scaleZ;
    }

    // Deep copy
    Mat4(const Mat4 &m) : nR(m.nR), nC(m.nC) {
        values.resize(nR * nC);
        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                values[i * nC + j] = m.values[i * nC + j];
            }
        }
    }

    // Deep copy
    void operator=(Mat4 const &m) {
        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                values[i * nC + j] = m.values[i * nC + j];
            }
        }
    }

    ~Mat4() = default;

    float get(const int &i, const int &j) const {
        return values[i * nC + j];
    }

    void set(const int &i, const int &j, const float &k) {
        values[i * nC + j] = k;
    }

    Mat4 *setZeros() {
        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                values[i * nC + j] = 0;
            }
        }
        return this;
    }

    Mat4 *setOnes() {
        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                values[i * nC + j] = 1;
            }
        }
        return this;
    }

    Mat4 *setIdentity() {
        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                values[i * nC + j] = (i == j);
            }
        }
        return this;
    }

    Mat4 *setUniform(float low, float high) {
        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                float randomNumber = (float) rand() / RAND_MAX;
                randomNumber = low + randomNumber * (high - low);
                values[i * nC + j] = randomNumber;
            }
        }
        return this;
    }

    // Transpose
    Mat4 operator~() const {
        Mat4 result;

        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                result.values[j * nR + i] = this->values[i * nC + j];
            }
        }

        return result;
    }

    // Element-wise add
    Mat4 operator+(Mat4 const &m) const {
        Mat4 result;

        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                result.values[i * nC + j] = this->values[i * nC + j] + m.values[i * nC + j];
            }
        }

        return result;
    }

    // Element-wise subtract
    Mat4 operator-(Mat4 const &m) const {
        Mat4 result;

        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                result.values[i * nC + j] = this->values[i * nC + j] - m.values[i * nC + j];
            }
        }

        return result;
    }

    // Matrix multiply
    Mat4 operator*(Mat4 const &m) const {
        Mat4 result;

        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < m.nC; ++j) {
                float elementSum = 0;
                for (int k = 0; k < nC; ++k) {
                    elementSum += this->values[i * nC + k] * m.values[k * m.nC + j];
                }
                result.values[i * m.nC + j] = elementSum;
            }
        }
        return result;
    }

    // Matrix multiply
    Vec3f operator*(Vec3f const &v) const {
        Vec3f result(values[0] * v[0] + values[1] * v[1] + values[2] * v[2],
                     values[4] * v[0] + values[5] * v[1] + values[6] * v[2],
                     values[8] * v[0] + values[9] * v[1] + values[10] * v[2]);

        return result;
    }

    // Element-wise scalar multiply
    Mat4 operator*(float const &value) const {
        Mat4 result;

        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                result.values[i * nC + j] = this->values[i * nC + j] * value;
            }
        }

        return result;
    }

    // Element-wise multiply
    Mat4 operator%(Mat4 const &m) const {
        Mat4 result;

        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                result.values[i * nC + j] = this->values[i * nC + j] * m.values[i * nC + j];
            }
        }

        return result;
    }

    void dumpColumnWise(float *array) const {
        for (int i = 0; i < nR; ++i) {
            for (int j = 0; j < nC; ++j) {
                array[i + j * nR] = this->values[i * nC + j];
            }
        }
    }

    Vec3 getTranslationVec3() const {
        return {values[3], values[7], values[11]};
    }
};

std::ostream &operator<<(std::ostream &out, const Mat4 &m) {
    for (int i = 0; i < m.nR; ++i) {
        for (int j = 0; j < m.nC; ++j) {
            out << m.values[i * m.nC + j] << " ";
        }
        out << std::endl;
    }
    return out;
}

#endif
