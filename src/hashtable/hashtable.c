
#include "hashtable.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

inline static bool ht_show_alloc_err = true;

#define IFNULL(VAR) if (unlikely((VAR) == NULL))
#define MEM_ALLOC_ERR(VAR, FUNC)                        \
IFNULL(VAR) {                                           \
  if (ht_show_alloc_err)                                \
    printf("Error: " #FUNC ": %s\n", strerror(errno));  \
}                                                       \
IFNULL(VAR)

/**
 * @param volatile void       *dest
 * @param volatile const void *src
 * @param size_t              n
 * @return volatile void *
 */
inline static volatile void *
volatile_memcpy(volatile void *dest, volatile const void *src, size_t n)
{
  volatile void *ret = dest;

#if !defined(DONT_USE_ERMSB) && defined(__linux__) && defined(__x86_64__)
  __asm__ volatile(
    "cld\n\t"
    "rep movsb"
    : "+D"(dest), "+S"(src), "+c"(n)
    :
    : "memory", "cc"
  );
#else
  volatile char *restrict _dest = dest;
  volatile const char *restrict _src  = src;
  for (size_t i = 0; i < n; i++) {
    _dest[i] = _src[i];
  }
#endif

  return ret;
}

/**
 * @param bool val
 * @return void
 */
void
ht_toggle_show_alloc_error(bool val)
{
  ht_show_alloc_err = val;
}


/**
 * @param const void *restrict key
 * @param size_t               len
 * @return size_t
 */
size_t
default_ht_hash_f(const void *restrict key, size_t len)
{
  size_t                  ret  = 0;
  const uint8_t *restrict ckey = (const uint8_t *restrict)key;

  for (size_t i = 0; i < len; i++) {
    ret += ((size_t)ckey[i]) << 4;
  }

  if (sizeof(void *) == 8) {
    return ret << 16;
  } else {
    return ret << 8;
  }
}


/**
 * @param size_t    bucket_num
 * @param size_t    group_size
 * @param HashFunc  hash_f
 * @return HashTable *
 */
HashTable *
create_ht(size_t bucket_num, size_t group_size, HashFunc hash_f)
{
  size_t ht_size     = sizeof(HashTable);
  size_t bucket_size = sizeof(HtBucket) * bucket_num;
  size_t total_size  = ht_size + bucket_size;

  uint8_t *heap = (uint8_t *)malloc(total_size);
  MEM_ALLOC_ERR(heap, create_ht) {
    return INSERT_FAILED;
  }
  memset(heap, 0, total_size);

  HashTable *ht   = (HashTable *)heap;
  ht->n           = 0;
  ht->group_size  = group_size;
  ht->bucket_num  = bucket_num;
  ht->hash_f      = (hash_f == NULL) ? default_ht_hash_f : hash_f;
  ht->buckets     = (HtBucket *)&(heap[ht_size]);

  return ht;
}


/**
 * @param HashTable  *ht
 * @param const void *restrict key
 * @param size_t     klen
 * @param const void *restrict val
 * @param size_t     vlen
 * @return InsertHtStatus
 */
InsertHtStatus
insert_item(HashTable *ht, const void *restrict key, size_t klen,
            const void *restrict val, size_t vlen)
{
  size_t   index;
  size_t   hash_val;
  size_t   bucket_num;
  size_t   group_size;
  uint8_t  *heap;
  HtValue  *value;
  HtItem   *item;
  HtBucket *target_bucket;
  InsertHtStatus status;

  /* Make sure hash function is not NULL. */
  assert(ht->hash_f != NULL);

  group_size    = ht->group_size;
  bucket_num    = ht->bucket_num;
  hash_val      = ht->hash_f(key, klen);
  index         = hash_val % bucket_num;
  target_bucket = &(ht->buckets[index]);


  if (likely(target_bucket->group == NULL)) {
    /* The bucket is empty. */
    status = INSERT_NO_COLLISION;

    GroupBucket *group;
    size_t alloc_size       = sizeof(GroupBucket);
    size_t key_item_size    = sizeof(HtItem) + klen;
    bool   has_unused_space = (group_size > key_item_size);

    alloc_size += has_unused_space ? group_size : key_item_size;
    group       = (GroupBucket *)malloc(alloc_size);

    MEM_ALLOC_ERR(group, insert_item) {
      return INSERT_FAILED;
    }

    if (has_unused_space) {
      group->m = group_size - key_item_size;
      group->f = &(group->data[key_item_size]);
      item     = (HtItem *)&(group->data[0]);
    } else {
      group->m = 0;
      group->f = NULL;
    }

    group->n    = 1;
    value       = (HtValue *)malloc(sizeof(HtValue) + vlen);

    MEM_ALLOC_ERR(value, insert_item) {
      return INSERT_FAILED;
    }

    item->klen  = klen;
    item->vptr  = value;
    value->vlen = vlen;
    target_bucket->group = group;
  } else {
    /* The bucket is not empty. */
    status = INSERT_COLLISION;
  }


  volatile_memcpy(value->val, val, vlen);
  volatile_memcpy(item->data, key, klen);

  return status;
}


int
main()
{
  return 0;
}
