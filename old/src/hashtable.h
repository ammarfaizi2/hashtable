
#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>

#define likely(EXPR)   __builtin_expect((EXPR), 1)
#define unlikely(EXPR) __builtin_expect((EXPR), 0)

typedef struct _Item
{
  void                *key;
  size_t              key_len;
  void                *data;
  size_t              data_len;
} Item;


typedef struct _Bucket
{
  Item                *item;
  struct _Bucket      *next;
} Bucket;


typedef struct _HashTable
{
  Bucket              *bucket;
  Bucket              **tail_bucket;
  size_t              size;
  size_t              item_c;
  size_t              (*hash_f)(const void *, size_t);
} HashTable;

size_t
default_ht_hash_f(const void *key, size_t len);

HashTable *
create_ht(size_t size, size_t (*hash_f)(const void *, size_t));

void
free_ht(HashTable *ht);

void
insert_item_ht(HashTable *ht, const void *key, size_t key_len,
               const void *data, size_t data_len);

void
insert_strstr_ht(HashTable *ht, const char *key, const char *data);

Bucket *
find_bucket_slot(Bucket *rbuck, const void *key, size_t key_len);

Item *
find_item_ht(HashTable *ht, const void *key, size_t key_len);

Item *
find_strstr_ht(HashTable *ht, const char *key);

#endif
