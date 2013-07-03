/******************************************************************************
 * Copyright 2008-2013 by Aerospike.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>

#include <citrusleaf/cf_alloc.h>
#include <aerospike/as_string.h>

/******************************************************************************
 * INLINE FUNCTIONS
 *****************************************************************************/

extern inline void          as_string_destroy(as_string * s);
extern inline char *        as_string_tostring(const as_string * s);
extern inline as_val *      as_string_toval(const as_string * s);
extern inline as_string *   as_string_fromval(const as_val * v);

/******************************************************************************
 * FUNCTIONS
 *****************************************************************************/

as_string * as_string_init(as_string * s, char * value, bool free) {
	as_val_init(&s->_, AS_STRING, false);
	s->free = free;
	s->value = value;
	s->len = SIZE_MAX;
	return s;
}

as_string * as_string_new(char * value, bool free) {
	as_string * s = (as_string *) malloc(sizeof(as_string));
	if (!s) return s;
	as_val_init(&s->_, AS_STRING, true);
	s->free = free;
	s->value = value;
	s->len = SIZE_MAX;
	return s;
}

size_t as_string_len(as_string * s) {
	if (s->value == NULL) {
		return 0;
	}
	if (s->len == SIZE_MAX) {
		s->len = strlen(s->value);
	}
	return s->len;
}

void as_string_val_destroy(as_val * v) {
	as_string * s = as_string_fromval(v);
	if ( s && s->value && s->free ) {
		free(s->value);
		s->value = NULL;
		s->free = false;
	}
}

uint32_t as_string_val_hashcode(const as_val * v) {
	as_string * s = as_string_fromval(v);
	if ( s == NULL || s->value == NULL) return 0;
	uint32_t hash = 0;
	int c;
	char * str = s->value;
	while ( (c = *str++) ) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}

char * as_string_val_tostring(const as_val * v) {
	as_string * s = (as_string *) v;
	if (s->value == NULL) return(NULL);
	size_t sl = as_string_len(s);
	size_t st = 3 + sl;
	char * str = (char *) malloc(sizeof(char) * st);
	if (!str) return str;
	*(str + 0) = '\"';
	strcpy(str + 1, s->value);
	*(str + 1 + sl) = '\"';
	*(str + 1 + sl + 1) = '\0';
	return str;
}
