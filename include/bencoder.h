#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    typedef enum
    {
        INTEGER,   // data = long long*
        STRING,    // data = char*
        LIST,      // data = BencodeStructure*
        DICTIONARY // data = BencodeMap*
    } StructureType;

    struct BencodeMap
    {
        char *key;
        struct BencodeStructure *value;
    };

    struct BencodeStructure
    {
        StructureType type;
        size_t length;
        void *data;
    };


    struct BencodeStructure *parseBencode(const char *data, size_t len);
    void freeBencode(struct BencodeStructure *struc);

#ifdef __cplusplus
}
#endif