/**
 * Vector class
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#define TOLERANCE 0.00001

class Vector {
    public:
        double x, y, z; 
        Vector(double a, double b, double c) {
            x = a; y = b; z = c;
        }
        Vector() { }
        Vector operator+(Vector v) {
            return Vector(x+v.x,y+v.y,z+v.z);
        }
        void normalize() {
            double mag_squared = x * x + y * y + z * z;
            if (fabs(mag_squared) > TOLERANCE && fabs(mag_squared - 1.0) > TOLERANCE) {
                double mag = sqrt(mag_squared);
                x /= mag;
                y /= mag;
                z /= mag;
            }
        }
};


#endif // _VECTOR_H_

