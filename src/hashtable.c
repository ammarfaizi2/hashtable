
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"

/**
 * @param const void  *key
 * @param size_t      len
 * @return size_t
 */
size_t
default_ht_hash_f(const void *key, size_t len)
{
  size_t              ret   = 0;
  const unsigned char *ckey = (const unsigned char *)key;

  for (size_t i = 0; i < len; i++) {
    ret += (size_t)ckey[i];
  }

  return ret;
}


/**
 * @param size_t size
 * @param size_t (*hash_f)(const void *, size_t)
 * @return HashTable *
 */
HashTable *
create_ht(size_t size, size_t (*hash_f)(const void *, size_t))
{
  size_t ht_siz        = sizeof(HashTable);
  size_t buck_siz      = sizeof(Bucket) * size;
  size_t tail_buck_siz = sizeof(Bucket *) * size;
  size_t total_siz     = ht_siz + buck_siz + tail_buck_siz;

  char         *heap   = (char *)malloc(total_siz);
  HashTable    *ht     = (HashTable *)heap;

  memset(heap, 0, total_siz);

  ht->bucket      = (Bucket *)&(heap[ht_siz]);
  ht->size        = size;
  ht->item_c      = 0;
  ht->hash_f      = (hash_f == NULL) ? default_ht_hash_f : hash_f;
  ht->tail_bucket = (Bucket **)&(heap[ht_siz + buck_siz]);

  return ht;
}


/**
 * @param HashTable *ht
 * @return void
 */
void
free_ht(HashTable *ht)
{
  size_t ht_siz = ht->size;

  for (size_t i = 0; i < ht_siz; i++) {
    Bucket *rbuck;
    Bucket *prev;

    if (ht->tail_bucket[i] == NULL) {
      continue;
    }

    /* Free the first bucket slot. */
    rbuck = &(ht->bucket[i]);
    free(rbuck->item);

    /* Now traverse the next slot. */
    rbuck = rbuck->next;

    while (rbuck != NULL) {
      free(rbuck->item);
      prev  = rbuck;
      rbuck = rbuck->next;
      free(prev);
    }
  }

  free(ht);
}


/**
 * @param HashTable  *ht
 * @param const void *key
 * @param size_t     key_len
 * @param const void *data
 * @param size_t     data_len
 * @return void
 */
void
insert_item_ht(HashTable *ht, const void *key, size_t key_len,
               const void *data, size_t data_len)
{
  char   *heap;
  size_t hash;
  size_t index;
  Item   *item;
  Bucket *tail;

  assert(ht->hash_f != NULL);

  hash  = ht->hash_f(key, key_len);
  index = hash % ht->size;
  heap  = (char *)malloc(sizeof(Item) + key_len + data_len);
  item  = (Item *)heap;

  item->key       = (void *)&(heap[sizeof(Item)]);
  item->key_len   = key_len;
  item->data      = (void *)&(heap[sizeof(Item) + key_len]);
  item->data_len  = data_len;

  memcpy(item->key, key, key_len);
  memcpy(item->data, data, data_len);

  tail = ht->tail_bucket[index];

  if (unlikely(tail != NULL)) {
    /* We have hash collision. */
    Bucket *key_col;

    key_col = find_bucket_slot(&(ht->bucket[index]), key, key_len);
    if (key_col == NULL) {
      tail->next = (Bucket *)malloc(sizeof(Bucket));
      tail       = tail->next;
    } else {
      /* Don't update tail, we only replace item. */
      free(key_col->item);
      key_col->item = item;
      return;
    }
  } else {
    tail       = &(ht->bucket[index]);
    assert(tail->next == NULL);
  }

  tail->item = item;
  tail->next = NULL;
  ht->tail_bucket[index] = tail;
}

/**
 * @param HashTable  *ht
 * @param const char *key
 * @param const char *data
 * @return void
 */
void
insert_strstr_ht(HashTable *ht, const char *key, const char *data)
{
  insert_item_ht(ht, key, strlen(key) + 1, data, strlen(data) + 1);
}


/**
 * @param Bucket     *rbuck
 * @param const void *key
 * @param size_t     key_len
 * @return Item *
 */
Bucket *
find_bucket_slot(Bucket *rbuck, const void *key, size_t key_len)
{
  do {

    if (unlikely(rbuck->item == NULL)) {
      goto next_slot;
    }

    if (rbuck->item->key_len != key_len) {
      goto next_slot;
    }

    if (!memcmp(rbuck->item->key, key, key_len)) {
      return rbuck;
    }

  next_slot:
    rbuck = rbuck->next;
  } while (rbuck != NULL);


  /* Not Found. */
  return NULL;
}


/**
 * @param HashTable  *ht
 * @param const void *key
 * @param size_t     key_len
 * @return Item *
 */
Item *
find_item_ht(HashTable *ht, const void *key, size_t key_len)
{
  size_t hash;
  size_t index;
  Bucket *rbuck;

  assert(ht->hash_f != NULL);

  hash  = ht->hash_f(key, key_len);
  index = hash % ht->size;

  if (ht->tail_bucket[index] == NULL) {
    /* Not Found. */
    return NULL;
  }

  rbuck = find_bucket_slot(&(ht->bucket[index]), key, key_len);
  return (rbuck != NULL) ? rbuck->item : NULL;
}

/**
 * @param HashTable  *ht
 * @param const void *key
 * @return Item *
 */
Item *
find_strstr_ht(HashTable *ht, const char *key)
{
  return find_item_ht(ht, key, strlen(key) + 1);
}


#include <time.h>

long long tdiff(struct timespec clock1, struct timespec clock2)
{
  clock2.tv_sec -= clock1.tv_sec;
  clock2.tv_nsec -= clock1.tv_nsec;
  if (clock2.tv_nsec < 0) {
    clock2.tv_sec -= 1;
    clock2.tv_nsec += 1000000000;
  }
  return 1000000000*clock2.tv_sec + clock2.tv_nsec;
}


extern void ht_construct(HashTable *ht);

/**
 * @return int
 */
int
main()
{
  Item *item;
  HashTable *ht;
  struct timespec clock1, clock2;

  ht = create_ht(500000, NULL);
  ht_construct(ht);

  clock_gettime(CLOCK_MONOTONIC, &clock1);
  item = find_strstr_ht(ht, "SutNmukuaOWUZJaHtaayfBrwXkEvyURBdwiTXneTIvsVSkPlhWbYBxRmUhWaFnbdWqyAnqsxXqifoZsJIDiFqWVvlBrilzUbqcgR");
  clock_gettime(CLOCK_MONOTONIC, &clock2);
  printf("%8lli ms\n", tdiff(clock1, clock2)/1000000);


  if (item != NULL) {
    printf("%s\n", (char *)item->data);
  } else {
    printf("Not Found\n");
  }

  free_ht(ht);
}

