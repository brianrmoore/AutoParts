#ifndef DualStream_H
#define DualStream_H

#include <iostream>
#include <ostream>


class DualStream {

    public:
        DualStream(std::ostream& os1, std::ostream& os2) : os1(os1), os2(os2) { }
        template<class T>
        DualStream& operator<<(const T& x)
            {
            os1 << x;
            os2 << x;
            return *this;
            }
            
    private:
        std::ostream&       os1;
        std::ostream&       os2;
};

#endif
