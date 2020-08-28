#ifndef GDZMATH_H
#define GDZMATH_H

#define PI 3.14159265358979323846264338327950288419716939937510


class GDZmath {

    public:

        static float clamp(float value, float max, float min){
            if(value > max){
                return max;
            }

            if(value < min){
                return min;
            }

            return value;
        }

};

#endif