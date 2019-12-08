#include <immintrin.h>

static inline int cirbuf_offer_opt(cirbuf_t *cb,
                                   const unsigned char *data,
                                   const int size)
{
    /* prevent buffer from getting completely full or over commited */
    if (cirbuf_unusedspace(cb) <= size)
        return 0;

    int i = 0, align = sizeof(__m256i);
    for(; i < (size & -align); i += align){
        __m256i buf = _mm256_lddqu_si256((__m256i  *) (data + i));
        _mm256_storeu_si256((__m256i *) (cb->data + cb->tail + i), buf);
    }

    memcpy(cb->data + cb->tail + i, data + i, size & (align -1));

    cb->tail += size;
    if (cb->size < cb->tail)
        cb->tail -= cb->size;
    return size;
}
