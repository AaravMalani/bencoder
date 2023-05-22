#include "../include/bencoder.h"

struct BencodeList
{
    void *data;
    size_t length;
    size_t capacity;
    size_t element_size;
};

struct BencodeList *bencodelist_new(size_t element_size)
{
    struct BencodeList *list = malloc(sizeof(struct BencodeList));
    list->data = malloc(element_size);
    list->capacity = 1;
    list->length = 0;
    list->element_size = element_size;
    return list;
}

void bencodelist_add(struct BencodeList *list, void *data)
{
    if (list->length + 1 > list->capacity)
    {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * list->element_size);
    }

    memcpy(((char *)list->data) + (list->element_size * list->length), data, list->element_size);
    list->length++;
}

void __freeBencode(struct BencodeStructure *struc, char freeParent)
{
    switch (struc->type)
    {
    case LIST:
        for (size_t i = 0; i < struc->length; i++)
        {
            __freeBencode(&((struct BencodeStructure *)struc->data)[i], 0);
        }
        break;
    case DICTIONARY:
        for (size_t i = 0; i < struc->length; i++)
        {
            __freeBencode(((struct BencodeMap *)struc->data)[i].value, 1);
            free(((struct BencodeMap *)struc->data)[i].key);
        }
        break;
    }
    free(struc->data);
    if (freeParent)
    {
        free(struc);
    }
}
void freeBencode(struct BencodeStructure *struc)
{
    __freeBencode(struc, 1);
}
struct BencodeStructure *__parseBencode(const char **dataSetter, const char *endptr)
{
    const char *data = *dataSetter;
    struct BencodeStructure *structure = malloc(sizeof(struct BencodeStructure));
    switch (*data++)
    {
    case 'i':
        structure->type = INTEGER;
        structure->data = malloc(sizeof(long long));
        *((long long *)structure->data) = strtoll(data, (char**) &data, 10);
        if (*data != 'e')
        {
            free(structure->data);
            free(structure);
            return NULL;
        }
        *dataSetter = data + 1;
        return structure;
    case 'l':;
        struct BencodeList *list = bencodelist_new(sizeof(struct BencodeStructure));
        while (*data != 'e')
        {
            struct BencodeStructure *struc = __parseBencode(&data, endptr);
            if (!struc)
            {
                free(list->data);
                free(list);
                free(structure);
                return NULL;
            }
            bencodelist_add(list, struc);
            free(struc);
        }
        *dataSetter = data + 1;
        structure->type = LIST;
        structure->length = list->length;
        structure->data = list->data;
        free(list);
        return structure;
    case 'd':;
        struct BencodeList *list2 = bencodelist_new(sizeof(struct BencodeMap));

        while (*data != 'e')
        {
            struct BencodeMap map;
            struct BencodeStructure *struc = __parseBencode(&data, endptr);
            if (!struc || struc->type != STRING)
            {
                free(list2->data);
                free(list2);
                free(structure);
                if (struc)
                {
                    free(struc->data);
                    free(struc);
                }
                return NULL;
            }
            struct BencodeStructure *struc2 = __parseBencode(&data, endptr);
            if (!struc2)
            {
                free(list2->data);
                free(list2);
                free(structure);
                free(struc->data);
                free(struc);
                return NULL;
            }
            map.key = struc->data;
            free(struc);
            map.value = struc2;
            bencodelist_add(list2, &map);
        }
        *dataSetter = data + 1;
        structure->type = DICTIONARY;
        structure->length = list2->length;
        structure->data = list2->data;
        free(list2);
        return structure;
    default:
        if ('0' > *(data - 1) || *(data - 1) > '9')
        {
            free(structure);
            return NULL;
        }
        size_t len = strtoull((data - 1), (char**) &data, 10);
        if (*data != ':')
        {
            free(structure);
            return NULL;
        }
        data++;
        if (data + len >= endptr)
        {

            free(structure);
            return NULL;
        }
        structure->type = STRING;
        structure->data = malloc(len + 1);
        structure->length = len;
        memcpy(structure->data, data, len);
        ((char *)structure->data)[len] = 0;
        *dataSetter = data + len;
        return structure;
    }
}
struct BencodeStructure *parseBencode(const char *data, size_t len)
{
    return __parseBencode(&data, data + len);
}