#ifndef GDZMATH_H
#define GDZMATH_H

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