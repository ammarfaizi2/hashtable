
#include "hashtable.h"

/**
 * @param size_t    size
 * @param size_t    group_size
 * @param HashFunc  func
 * @return HashTable *
 */
HashTable *
create_ht(size_t size, size_t group_size, HashFunction func)
{
  size_t ht_size     = sizeof(HashTable);
  size_t bucket_size = sizeof(HtBucket) * size;
  size_t total_size  = ht_size + bucket_size;

  char      *heap = (char *)malloc(total_size);
  HashTable *ht   = (HashTable *)heap;

  memset(heap, 0, total_size);

  if (group_size)

  ht->buckets    = (HtBucket *)&(heap[ht_size]);
  ht->size       = size;
  ht->group_size = group_size;

}
