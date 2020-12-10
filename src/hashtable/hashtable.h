
#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define likely(EXPR)   __builtin_expect((EXPR), 1)
#define unlikely(EXPR) __builtin_expect((EXPR), 0)

#define OFFSETOF(TYPE, MEMBER) ((size_t)&(((TYPE *)0)->MEMBER))

#define HT_DEFAULT_GROUP_SIZE (4096ul)

typedef struct _HtValue     HtValue;
typedef struct _HtItem      HtItem;
typedef struct _GroupBucket GroupBucket;
typedef struct _HtBucket    HtBucket;
typedef struct _HashTable   HashTable;
typedef size_t (*HashFunc)(const void *restrict key, size_t len);

struct _HtValue
{
  size_t      vlen;       /* The length of value (in bytes). */
  char        val[1];     /* Item value.                     */
};

struct _HtItem
{
  size_t      klen;       /* The length of the key.     */
  HtValue     *vptr;      /* Pointer to the value.      */
  char        data[1];    /* Struct hack contains keys. */
};

struct _GroupBucket
{
  GroupBucket  *next;     /* Next group.                         */
  size_t       n;         /* The number of items in this bucket. */
  size_t       m;         /* Unused space (in bytes).            */
  char         *f;        /* Pointer to unused space.            */
  char         data[1];   /* Struct hack contains items.         */
};

struct _HtBucket
{
  size_t       item_size; /* The number of items in this bucket. */
  GroupBucket  *group;    /* Pointer to the first group.         */
};

struct _HashTable
{
  size_t    n;            /* The number of items in this hashtable.   */
  size_t    group_size;   /* Max bytes for each group.                */
  size_t    bucket_num;   /* The number of buckets in this hashtable. */
  HashFunc  hash_f;       /* The hash function.                       */
  HtBucket  *buckets;     /* Array of buckets.                        */
};

typedef enum __attribute__((packed))
{
  INSERT_FAILED       = 0, /* Insert failed. Run out of memory? */
  INSERT_NO_COLLISION = 1, /* O(1) insert.                      */
  INSERT_COLLISION    = 1, /* Hash collision occured.           */
  INSERT_REPLACE_KEY  = 2, /* Hash and key collision occured.   */
} InsertHtStatus;

typedef enum __attribute__((packed))
{
  DELETE_NO_KEY = 0, /* There is no such a key in hashtable. */
  DELETE_OK     = 1, /* Key is found and deleted.            */
} DeleteHtStatus;

void
ht_toggle_show_alloc_error(bool val);

HashTable *
create_ht(size_t bucket_num, size_t group_size, HashFunc hash_f);

InsertHtStatus
insert_item(HashTable *ht, const void *restrict key, size_t klen,
            const void *restrict val, size_t vlen);

InsertHtStatus
insert_str(HashTable *ht, const void *restrict keystr,
           const void *restrict val, size_t vlen);

InsertHtStatus
insert_strstr(HashTable *ht, const void *restrict keystr,
              const void *restrict valstr);

#endif /* #ifndef __HASHTABLE_H */
