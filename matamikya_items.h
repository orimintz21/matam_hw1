#ifndef ITEMS_H_
#define ITEMS_H_

#include "matamikya_product.h"
#include "matamikya.h"
#include "set.h"
#include "amount_set.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Items_t *Items;

void* itCopyItemVoid(void* old_items);

Items itCreateItem();

void itSetItem(Items item, unsigned int id);

Items itCopyItem(Items old_item);

void itFreeItem(Items items);

void itFreeItemVoid(void* items);

int itComperItems(Items items1, Items items2);

int itComperItemsVoid(void* item1, void* item2);


int itGetId(Items item);

#endif