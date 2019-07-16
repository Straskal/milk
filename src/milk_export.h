#ifndef _MILK_EXPORT_H_
#define _MILK_EXPORT_H_

    #if defined(_MSC_VER)
        #define MILK_EXPORT __declspec(dllexport)
    #elif defined(__GNUC__)
        #define MILK_EXPORT __attribute__((visibility("default")))
    #endif

#endif