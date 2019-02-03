/*
 *  functionPtr.hpp
 *  IG
 *
 *  Created by Sach Patel on 03/02/2019.
 *  Copyright © 2019 Sach Patel. All rights reserved.
 */

#ifndef functionPtr_hpp
#define functionPtr_hpp

#include <memory>

namespace FP {
    
    /* Templates to define the &deleter object for std::unique_ptr<T, void(*deleter)(T*)> */
    template <class T1, class T2> struct _FUNCPTR { typedef T1 (*functionPtr) (T2); };
    template <typename T> using delPtr = typename _FUNCPTR<void, T>::functionPtr;
    
}

namespace IG {
    
    /* Type define uniquePtr for unique_ptr with custom deleter */
    template <typename T> using uniquePtr = typename std::unique_ptr<T, FP::delPtr<T*> >;
    /* Inline function for getting raw pointer from unique pointer */
    template <typename T> static inline constexpr T * raw (const uniquePtr<T> &sp) { return sp.get (); }
    
}


#endif /* functionPtr_hpp */
