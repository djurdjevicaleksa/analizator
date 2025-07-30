#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include <cstddef>

/*
    Base class not to be used by itself, but as an interface.
*/

namespace Descriptors {
    
    class Descriptor {
    protected:

        std::size_t length;
    
    public:
        Descriptor(std::size_t len) : length(len) {}

        Descriptor() = delete;
        Descriptor(Descriptor&) = delete;
        Descriptor& operator=(Descriptor&) = delete;

        Descriptor(Descriptor&&) = default;
        virtual ~Descriptor() = default;
        
        virtual void print(std::size_t) const;
    };
}

#endif // _DESCRIPTOR_H