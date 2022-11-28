#include "matamikya_product.h"
#include "matamikya_items.h"
#include "matamikya.h"
#include "set.h"
#include "amount_set.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>



struct Items_t
{
    unsigned int id;
};

Items itCreateItem()
{
    Items new_item = malloc(sizeof(*new_item));
    if(new_item == NULL)
    {
        return NULL;
    }
    return new_item;
}

void itSetItem(Items item, unsigned int id)
{
    if(item == NULL)
    {
        return;
    }
    item->id = id;
}

void* itCopyItemVoid(void* old_item)
{
    return (void*)itCopyItem((Items)old_item);
}

Items itCopyItem(Items old_item)
{
    if(old_item == NULL)
    {
        return NULL;
    }
    Items new_items = malloc(sizeof(*new_items));
    if(new_items == NULL)
    {
        return NULL;
    }
    new_items->id = old_item->id;
    return new_items;
}

void itFreeItemVoid(void* items)
{   
    itFreeItem((Items)items);
    return;
}


void itFreeItem(Items items)
{
    if(items == NULL)
    {
        return;
    }
    free(items);
    items = NULL;
    return;
}

int itComperItemsVoid(void* item1, void* item2)
{
    return itComperItems((Items) item1, (Items) item2);
}

int itComperItems(Items items1, Items items2)
{
    if(items1 == NULL || items2 == NULL)
    {
        return 0;
    }
    if(items1->id > items2->id)
    {
        return 1;
    }
    if(items1->id < items2->id)
    {
        return -1;
    }
    return 0;
}

int itGetId(Items item)
{
    if(item == NULL)
    {
        return 0;
    }
    return item->id;
}