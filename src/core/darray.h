#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    void* data;
    size_t element_size;
    size_t i;
    size_t size;
} darray;

void darray_init(darray* arr, size_t element_size, size_t size);
void darray_push(darray* arr, void* element);
void darray_insert(darray* arr, void* element, size_t index);
void darray_remove(darray* arr, size_t index);
void* darray_get(darray* arr, size_t index);
void darray_free(darray* arr);

#endif