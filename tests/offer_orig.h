static inline int cirbuf_offer_orig(cirbuf_t *cb,
                                   const unsigned char *data,
                                   const int size)
{
    /* prevent buffer from getting completely full or over commited */
    if (cirbuf_unusedspace(cb) <= size)
        return 0;
    
    long mask = (long) (size - cb->size + cb->tail) >> 63;
    const size_t part1 = (size & mask) + ((cb->size - cb->tail) & ~mask);
    const size_t part2 = size - part1;
    memcpy(cb->data + cb->tail, data, part1);
    memcpy(cb->data, data + part1, part2);

    cb->tail += size;
    if (cb->size < cb->tail)
        cb->tail -= cb->size;
    return size;
}