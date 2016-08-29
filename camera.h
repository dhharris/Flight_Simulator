/**
 * Camera class
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector.h"
#include "quaternion.h"

class Camera {
    public:
        Quaternion rotation;	                        /* Direction */
        Vector pos;				        /* Position */
        double speed;			                /* Forward speed */
        Camera() {
            pos = Vector(0, 0.05, 0);
            rotation = Quaternion(1.0, 0.0, 0.0, 0.0);
            rotation.normalise();
            speed = 0.01;
        }
        void forward(double t) {
            pos = pos + rotation * Vector(t, 0.0, 0.0);
        }
        void up(double t) {
            pos.y += t;
        }
        void yaw(double r) {
            Quaternion nrot(r, 0.0, 1.0, 0.0);
            rotation = nrot * rotation;
            rotation.normalise();
        }
        void roll(double r) {
            Vector v = rotation * Vector(1.0, 0.0, 0.0);
            Quaternion nrot(r, v.x, v.y, v.z);
            nrot.normalise();
            rotation = nrot * rotation;
            rotation.normalise();
        }
        void pitch(double r) {
            Vector v = rotation * Vector(0.0, 0.0, 1.0);
            Quaternion nrot(r, v.x, v.y, v.z);
            nrot.normalise();
            rotation = nrot * rotation;
            rotation.normalise();
        }
        void tick(double seconds) {
            forward(speed * seconds);
        }
};

#endif // _CAMERA_H_
