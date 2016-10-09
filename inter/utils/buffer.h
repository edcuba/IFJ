/* buffer.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jakub Kulich <xkulic03@stud.fit.vutbr.cz>
 */

#ifndef IFJ_BUFFER_H
#define IFJ_BUFFER_H

typedef struct _dyn_buffer dyn_buffer;
struct _dyn_buffer {
    char *buffer;
    int top;
    int size;
};

dyn_buffer *dyn_buffer_init(int init_size);
dyn_buffer *dyn_buffer_append(dyn_buffer *b, char character);
char *dyn_buffer_get_content(dyn_buffer *b);
int dyn_buffer_get_size(dyn_buffer *b);
void dyn_buffer_clear(dyn_buffer *b);
void dyn_buffer_free(dyn_buffer *b);

#endif //IFJ_BUFFER_H
