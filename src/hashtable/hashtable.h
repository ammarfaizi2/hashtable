
#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include <stddef.h>

typedef struct _GroupBucket GroupBucket;
typedef struct _HtBucket    HtBucket;
typedef struct _HashTable   HashTable;
typedef struct _HtItem      HtItem;

struct _HtItem
{
  size_t      key_len;    /* The length of the key.              */
  size_t      val_len;    /* The length of the value.            */
  char        data[1];    /* Struct hack contains key + val.     */
};

struct _GroupBucket 
{
  GroupBucket  *next;     /* Next group.                         */
  size_t       num_item;  /* The number of items in this bucket. */
  char         items[1];  /* Struct hack contains items.         */
};

struct _HtBucket
{
  size_t       item_size; /* The number of items in this bucket.  */
  GroupBucket  *group;    /* The first group of this bucket.      */
};

struct _HashTable
{
  size_t    size;         /* The number of elements in this hashtable.    */
  size_t    bucket_size;  /* The number of buckets in this hashtable.     */
  size_t    group_size;   /* The number of elements in consecutive block. */
  HtBucket  *buckets;     /* The buckets array.                           */
};

#endif
