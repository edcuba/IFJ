/* buffer.h
 *
 * Buffer is used as more primitive string
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

/**
 * Initialize dynamic buffer
 *
 * @param init_size Initial size of buffer
 * @return pointer to buffer
 */
dyn_buffer *dyn_buffer_init(int init_size);

/**
 * Character is appended to buffer
 *
 * @param b Buffer where char is appended
 * @param character character
 * @return Pointer to buffer in case its reallocated
 */
dyn_buffer *dyn_buffer_append(dyn_buffer *b, char character);

/**
 * Function returns pointer to array of chars in buffer and append null byte
 * to the end
 *
 * @param b buffer to read
 * @return array of chars
 */
char *dyn_buffer_get_content(dyn_buffer *b);

/**
 * Get size of array of chars in buffer
 *
 * @param b buffer
 * @return size of array of chars in buffer
 */
int dyn_buffer_get_size(dyn_buffer *b);

/**
 * Clear buffer, so existing string in buffer is deleted
 *
 * @param b buffer
 */
void dyn_buffer_clear(dyn_buffer *b);

/**
 * Buffer is completely destroyed and memory freed
 *
 * @param b buffer to free
 */
void dyn_buffer_free(dyn_buffer *b);

#endif //IFJ_BUFFER_H
