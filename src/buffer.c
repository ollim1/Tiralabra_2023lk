#include "../include/buffer.h"
#include "../include/ealloc.h"

void buffer_resize(Buffer *buf, size_t new_size);

Buffer *new_buffer()
{
    /*
     * create an empty buffer with a BUFSIZE initial size
     */
    Buffer *ret = mmalloc(sizeof(Buffer));
    unsigned char *data = mcalloc(BUFSIZE, 1);
    ret->data = data;
    ret->size = BUFSIZE;
    ret->len = 0;
    return ret;
}

void delete_buffer(Buffer *buf)
{
    /*
     * delete buffer, ignoring any any pointers contained within its elements
     */
    if (buf) {
        if (buf->data)
            free(buf->data);
        free(buf);
    }
}

Buffer *buffer_copy(Buffer *buf)
{
    /*
     * duplicate a Buffer
     */
    if (!buf)
        err_quit("null pointer duplicating buffer");
    return buffer_copyl(buf, buf->len);
}

Buffer *buffer_copyl(Buffer *buf, size_t len)
{
    /*
     * duplicate a Buffer
     */
    if (!buf)
        err_quit("null pointer duplicating buffer");

    Buffer *ret = new_buffer();
    buffer_append(ret, buf->data, len < buf->len ? len : buf->len);
    return ret;
}

void buffer_pad(Buffer *buf, size_t len)
{
    /*
     * pad buffer with len zeroes
     */
    if (!buf)
        err_quit("null pointer when resizing buffer");

    size_t newSize = buf->size;
    while (buf->len + len > newSize)
        newSize *= 2;

    if (newSize > buf->size) {
        buffer_resize(buf, newSize);
    }
    memset(buf->data + buf->len, 0, len);
    buf->len += len;
}

void buffer_truncate(Buffer *buf)
{
    /*
     * truncate Buffer to only the last character
     */
    if (!buf)
        err_quit("null pointer truncating buffer");
    if (buf->len < 1)
        return;
    buf->data[0] = buf->data[buf->len - 1];
    buf->len = 1;
}

void buffer_shrink(Buffer *buf)
{
    /*
     * shrink buffer length by one
     * boilerplate for better readability
     */

    if (!buf)
        err_quit("null pointer truncating buffer");
    if (buf->len < 1)
        return;
    buf->len--;
}

void buffer_append(Buffer *dest, unsigned char *src, size_t len)
{
    if (!dest || !src)
        err_quit("null pointer when appending to buffer");

    size_t newSize = dest->size;
    while (dest->len + len > newSize)
        newSize *= 2;

    if (newSize > dest->size) {
        buffer_resize(dest, newSize);
    }
    memcpy(dest->data + dest->len, src, len);
    dest->len += len;
}

void buffer_resize(Buffer *buf, size_t newSize)
{
    if (!buf)
        err_quit("cannot expand buffer: null pointer");

    unsigned char *newData = mrealloc(buf->data, newSize);
    buf->data = newData;
    buf->size = newSize;
}

void buffer_clear(Buffer *buf)
{
    if (!buf)
        err_quit("null pointer in buffer_clear");
    buf->len = 0;
}

BufferReader *buffer_createReader(Buffer *buffer)
{
    if (!buffer)
        err_quit("null pointer creating BufferReader");

    BufferReader *ret = mmalloc(sizeof(BufferReader));
    ret->data = buffer;
    ret->pos = 0;
    return ret;
}

void delete_bufferreader(BufferReader *reader)
{
    if (reader)
        free(reader);
}

ssize_t bufferreader_read(BufferReader *reader, unsigned char *dst, size_t len)
{
    if (!reader || !dst)
        err_quit("null pointer on bufferreader_read");
    ssize_t total;
    if (reader->pos >= reader->data->len)
        return -1;
    for (total = 0; total < len && reader->pos < reader->data->len; total++, reader->pos++) {
        dst[total] = reader->data->data[reader->pos];
    }
    return total;
}

int bufferreader_isFinal(BufferReader *reader)
{
    if (!reader)
        err_quit("null pointer on bufferreader_isFinished");

    return reader->pos >= reader->data->len;
}
