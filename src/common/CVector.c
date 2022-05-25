/**
 * By Hamid Alipour from https://web.archive.org/web/20111216154706/http://codingrecipes.com/implementation-of-a-CVector-data-structure-in-c
 * Modified by James Michael Armstrong(https://github.com/BlazesRus)
 */
 
#include "CVector.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

//CVector Macro for checking if currently has enough allocated elements to add the element without allocating more space
#define CVECTOR_HASSPACE(v)  (((v)->num_elems + 1) <= (v)->num_alloc_elems)
//CVector Macro for checking if element is within bounds
#define CVECTOR_INBOUNDS(i)	(((int) i) >= 0 && (i) < (v)->num_elems)
//CVector Macro for returning element index
#define CVECTOR_INDEX(i)		((char *) (v)->elems + ((v)->elem_size * (i)))
 
extern void CVector_init(CVector *v, size_t elem_size, size_t init_size, void (*free_func)(void *))
{
	v->elem_size = elem_size;
	v->num_alloc_elems = init_size;//(int) init_size > 0 ? init_size : 4;//Initial size of 4 if size is less than 0
	v->num_elems = 0;
	v->elems = malloc(elem_size * v->num_alloc_elems);
	assert(v->elems != NULL);
	v->free_func = free_func != NULL ? free_func : NULL;
}
 
extern void CVector_dispose(CVector *v)
{
	size_t i;
 
	if (v->free_func != NULL) {
		for (i = 0; i < v->num_elems; ++i) {
			v->free_func(CVECTOR_INDEX(i));
		}
	}
 
	free(v->elems);
}
 
extern void CVector_copy(CVector *v1, CVector *v2)
{
	v2->num_elems = v1->num_elems;
	v2->num_alloc_elems = v1->num_alloc_elems;
	v2->elem_size = v1->elem_size;
 
	v2->elems = realloc(v2->elems, v2->num_alloc_elems * v2->elem_size);
	assert(v2->elems != NULL);
 
	memcpy(v2->elems, v1->elems, v2->num_elems * v2->elem_size);
}
 
extern void CVector_insert(CVector *v, void *elem, size_t index)
{
	void *target;
 
	if ((int) index > -1) {
		if (!CVECTOR_INBOUNDS(index))
			return;
		target = CVECTOR_INDEX(index);
	} else {
		if (!CVECTOR_HASSPACE(v))
			CVector_grow(v, 0);
		target = CVECTOR_INDEX(v->num_elems);
		++v->num_elems; /* Only grow when adding a new item not when inserting in a spec indx */
	}
 
	memcpy(target, elem, v->elem_size);
}
 
extern void CVector_insert_at(CVector *v, void *elem, size_t index)
{
	if ((int) index < 0)
		return;
 
	if (!CVECTOR_HASSPACE(v))
		CVector_grow(v, 0);
 
	if (index < v->num_elems)
		memmove(CVECTOR_INDEX(index + 1), CVECTOR_INDEX(index), (v->num_elems - index) * v->elem_size);
 
	/* 1: we are passing index so insert won't increment this 2: insert checks INBONDS... */
	++v->num_elems;
 
	CVector_insert(v, elem, index);
}
 
extern void CVector_push(CVector *v, void *elem)
{
	CVector_insert(v, elem, -1);
}
 
extern void CVector_pop(CVector *v, void *elem)
{
	memcpy(elem, CVECTOR_INDEX(v->num_elems - 1), v->elem_size);
	v->num_elems--;
}
 
extern void CVector_shift(CVector *v, void *elem)
{
	memcpy(elem, v->elems, v->elem_size);
	memmove(CVECTOR_INDEX(0), CVECTOR_INDEX(1), v->num_elems * v->elem_size);
 
	--v->num_elems;
}
 
extern void CVector_unshift(CVector *v, void *elem)
{
	if (!CVECTOR_HASSPACE(v))
		CVector_grow(v, v->num_elems + 1);
 
	memmove(CVECTOR_INDEX(1), v->elems, v->num_elems * v->elem_size);
	memcpy(v->elems, elem, v->elem_size);
 
	++v->num_elems;
}
 
extern void CVector_transpose(CVector *v, size_t index1, size_t index2)
{
	CVector_swap(CVECTOR_INDEX(index1), CVECTOR_INDEX(index2), v->elem_size);
}
 
static void CVector_grow(CVector *v, size_t size)
{
	if (size > v->num_alloc_elems)
		v->num_alloc_elems = size;
	else
		v->num_alloc_elems *= 2;
 
	v->elems = realloc(v->elems, v->elem_size * v->num_alloc_elems);
	assert(v->elems != NULL);
}
 
extern void CVector_get(CVector *v, size_t index, void *elem)
{
	assert((int) index >= 0);
 
	if (!CVECTOR_INBOUNDS(index)) {
		elem = NULL;
		return;
	}
 
	memcpy(elem, CVECTOR_INDEX(index), v->elem_size);
}
 
extern void CVector_get_all(CVector *v, void *elems)
{
	memcpy(elems, v->elems, v->num_elems * v->elem_size);
}
 
extern void CVector_remove(CVector *v, size_t index)
{
	assert((int) index > 0);
 
	if (!CVECTOR_INBOUNDS(index))
		return;
 
	memmove(CVECTOR_INDEX(index), CVECTOR_INDEX(index + 1), v->elem_size);
	--v->num_elems;
}
 
extern void CVector_remove_all(CVector *v)
{
	v->num_elems = 0;
	v->elems = realloc(v->elems, v->num_alloc_elems);
	assert(v->elems != NULL);
}
 
extern size_t CVector_length(CVector *v)
{
	return v->num_elems;
}
 
extern size_t CVector_size(CVector *v)
{
	return v->num_elems * v->elem_size;
}
 
extern void CVector_cmp_all(CVector *v, void *elem, int (*cmp_func)(const void *, const void *))
{
	size_t i;
	void *best_match = CVECTOR_INDEX(0);
 
	for (i = 1; i < v->num_elems; i++)
		if (cmp_func(CVECTOR_INDEX(i), best_match) > 0)
			best_match = CVECTOR_INDEX(i);
 
	memcpy(elem, best_match, v->elem_size);
}
 
extern void CVector_qsort(CVector *v, int (*cmp_func)(const void *, const void *))
{
	qsort(v->elems, v->num_elems, v->elem_size, cmp_func);
}
 
static void CVector_swap(void *elemp1, void *elemp2, size_t elem_size)
{
	void *tmp = malloc(elem_size);
 
	memcpy(tmp, elemp1, elem_size);
	memcpy(elemp1, elemp2, elem_size);
	memcpy(elemp2, tmp, elem_size);
 
    free(tmp); /* Thanks to gromit */
}