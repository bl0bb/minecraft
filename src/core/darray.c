#include "darray.h"

void darray_init(darray* arr, size_t element_size, size_t size) {
    arr->data = malloc(element_size * size);
    arr->element_size = element_size;
    arr->i = 0;
    arr->size = size;
}

void darray_push(darray* arr, void* element) {
    if (arr->i == arr->size) {
        arr->size *= 2;
        arr->data = realloc(arr->data, arr->element_size * arr->size);
    }
    memcpy((char*)arr->data + arr->i * arr->element_size, element, arr->element_size);
    arr->i++;
}

void darray_insert(darray* arr, void* element, size_t index) {
    if (index > arr->i) return;
    if (arr->i == arr->size) {
        arr->size *= 2;
        arr->data = realloc(arr->data, arr->element_size * arr->size);
    }
    memmove((char*)arr->data + (index + 1) * arr->element_size,
            (char*)arr->data + index * arr->element_size,
            (arr->i - index) * arr->element_size);
    memcpy((char*)arr->data + index * arr->element_size, element, arr->element_size);
    arr->i++;
}

void darray_remove(darray* arr, size_t index) {
    if (index >= arr->i) return;
    memmove((char*)arr->data + index * arr->element_size,
            (char*)arr->data + (index + 1) * arr->element_size,
            (arr->i - index - 1) * arr->element_size);
    arr->i--;
}

void* darray_get(darray* arr, size_t index) {
    if (index >= arr->i) return NULL;
    return (char*)arr->data + index * arr->element_size;
}

void darray_free(darray* arr) {
    free(arr->data);
    arr->data = NULL;
    arr->i = 0;
    arr->size = 0;
}