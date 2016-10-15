#include <stdlib.h>
#include "buffer.h"

/* buffer.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jakub Kulich <xkulic03@stud.fit.vutbr.cz>
 */

dyn_buffer *dyn_buffer_init(int init_size) {
    dyn_buffer *buffer = malloc(sizeof(dyn_buffer));
    if (buffer == NULL) {
        return NULL;
    }

    buffer->buffer = malloc((unsigned) init_size);
    if (buffer->buffer == NULL) {
        free(buffer);
        return NULL;
    }

    buffer->size = init_size;
    buffer->top = -1;

    return buffer;
}

dyn_buffer *dyn_buffer_append(dyn_buffer *b, char character) {
    if (b->top == b->size - 1) {
        dyn_buffer *nb = realloc(b, (unsigned)b->size*2);
        if (nb == NULL) {
            free(b);
            return NULL;
        } else {
            b = nb;
            b->size = b->size*2;
        }
    }

    b->top++;
    b->buffer[b->top] = character;
    return b;
}

char *dyn_buffer_get_content(dyn_buffer *b) {
    if (b->buffer[b->top] != '\0') {
        dyn_buffer_append(b, '\0');
    }
    return b->buffer;
}

int dyn_buffer_get_size(dyn_buffer *b) {
    if (b->buffer[b->top] != '\0') {
        return b->top + 2;
    } else {
        return b->top + 1;
    }
}

void dyn_buffer_clear(dyn_buffer *b) {
    b->top = -1;
}

void dyn_buffer_free(dyn_buffer *b) {
    free(b->buffer);
    free(b);
}