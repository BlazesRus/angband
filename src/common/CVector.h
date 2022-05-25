/**
 * By Hamid Alipour from https://web.archive.org/web/20111216154706/http://codingrecipes.com/implementation-of-a-CVector-data-structure-in-c
 * Modified by James Michael Armstrong(https://github.com/BlazesRus)
 */
 
#ifndef __CVECTORH__
#define __CVECTORH__
 
typedef struct CVector {
	void *elems;
	size_t elem_size;
	size_t num_elems;
	size_t num_alloc_elems;
    void (*free_func)(void *);
} CVector;
 
extern void CVector_init(CVector *, size_t, size_t, void (*free_func)(void *));
extern void CVector_dispose(CVector *);
extern void CVector_copy(CVector *, CVector *);
extern void CVector_insert(CVector *, void *, size_t index);
extern void CVector_insert_at(CVector *, void *, size_t index);
extern void CVector_push(CVector *, void *);
extern void CVector_pop(CVector *, void *);
extern void CVector_shift(CVector *, void *);
extern void CVector_unshift(CVector *, void *);
extern void CVector_get(CVector *, size_t, void *);
extern void CVector_remove(CVector *, size_t);
extern void CVector_transpose(CVector *, size_t, size_t);
extern size_t CVector_length(CVector *);
extern size_t CVector_size(CVector *);
extern void CVector_get_all(CVector *, void *);
extern void CVector_cmp_all(CVector *, void *, int (*cmp_func)(const void *, const void *));
extern void CVector_qsort(CVector *, int (*cmp_func)(const void *, const void *));
static void CVector_grow(CVector *, size_t);
static void CVector_swap(void *, void *, size_t);
 
#endif