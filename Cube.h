#ifndef CUBE_H
#define CUBE_H

namespace GEII
{
    class Cube
    {

    */
        class Cube : public Box
        {
        public:
            Square(float width, float height);
            virtual ~Square(void);
        protected :
            float mWidth, mHeight;
          //  CubeUVType mUVPattern;
        };
        public:
            Cube(float cSize);
            virtual ~Cube(void);

        protected:

        private:
    };
}

#endif // CUBE_H
