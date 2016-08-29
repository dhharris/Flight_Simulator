/**
 * Quaternion class
 */

#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#define TOLERANCE 0.00001

class Quaternion {
    public:
        double w, x, y, z;
        Quaternion(double a, double b, double c, double d) {
            w = a; x = b; y = c; z = d;
        }
        Quaternion() { }
        void normalise() {
            double mag_squared = w * w + x * x + y * y + z * z;
            if (fabs(mag_squared) > TOLERANCE && fabs(mag_squared - 1.0) > TOLERANCE) {
                double mag = sqrt(mag_squared);
                w /= mag;
                x /= mag;
                y /= mag;
                z /= mag;
            }
        };
        Quaternion conjugate() const {
            return Quaternion(w,-x,-y,-z);
        }
        Quaternion operator*(const Quaternion &rq) const {
            return Quaternion(
                    w * rq.w - x * rq.x - y * rq.y - z * rq.z,
                    w * rq.x + x * rq.w + y * rq.z - z * rq.y,
                    w * rq.y + y * rq.w + z * rq.x - x * rq.z,
                    w * rq.z + z * rq.w + x * rq.y - y * rq.x
                    );
        }
        Vector operator*(const Vector &vec) const {
            Vector vn(vec.x,vec.y,vec.z);
            vn.normalize();
            Quaternion vecQuat, resQuat;
            vecQuat.x = vn.x;
            vecQuat.y = vn.y;
            vecQuat.z = vn.z;
            vecQuat.w = 0.0;
            resQuat = vecQuat * conjugate();
            resQuat = *this * resQuat;
            return Vector(resQuat.x,resQuat.y,resQuat.z);
        }
};

#endif // _QUATERNION_H_
