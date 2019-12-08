#include <immintrin.h>
#include <stdint.h>

static inline int cirbuf_offer_opt(cirbuf_t *cb,
                                   const unsigned char *data,
                                   const int size)
{
    /* prevent buffer from getting completely full or over commited */
    if (cirbuf_unusedspace(cb) <= size)
        return 0;

    void *base = cb->data + cb->tail;
    int i = 0, align = sizeof(__m256i);
    for(; i < (size & -align); i += align){
        __m256i buf = _mm256_lddqu_si256((__m256i  *) (data + i));
        _mm256_storeu_si256((__m256i *) (base + i), buf);
    }
    
    if(size & 16){ 
        __m128i buf2 = _mm_lddqu_si128 ((__m128i *) (data + i));
        _mm_storeu_si128((__m128 *) (base + i), buf2);
        i += 16;
    }
    if(size & 8){
        *(int64_t *) (base + i) = *(int64_t *) (data + i);
        i += 8;
    }
    if(size & 4){
        *(int32_t *) (base + i) = *(int32_t *) (data + i);
        i += 4;
    }
    if(size & 2){
        *(int16_t *) (base + i) = *(int16_t *) (data + i);
        i += 2;
    }
    if(size & 1)
        *(int8_t *) (base + i) = *(int8_t *) (data + i);
    
    cb->tail += size;
    if (cb->size < cb->tail)
        cb->tail -= cb->size;
    return size;
}
