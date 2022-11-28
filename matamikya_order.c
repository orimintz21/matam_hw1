#include "matamikya.h"
#include "amount_set.h"
#include "set.h"
#include "matamikya_product.h"
#include "matamikya_order.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct Order_t
{
    unsigned int id;
    AmountSet items;
};

Order orCreatOrder()
{
    Order new_order = malloc(sizeof(*new_order));
    if(new_order == NULL)
    {
        return NULL;
    }
    new_order->id =0;
    
    new_order->items = asCreate(itCopyItemVoid, itFreeItemVoid, itComperItemsVoid);
    if(new_order->items == NULL)
    {
        free(new_order);
        new_order = NULL;
        return NULL;
    } 

    return new_order;
}


void orGetIdToOrder(Order order, unsigned int id)
{
    if(order == NULL)
    {
        return;
    }
    order->id = id;
    return;
}


SetElement orCopyOrderVoid(SetElement old_order)
{
    Order order = (Order)old_order;
    if(order == NULL)
    {
        return NULL;
    }
    Order new_order = malloc(sizeof(*new_order));
    if(new_order == NULL)
    {
        return NULL;
    }
    new_order->id = order->id;
    if(order->items == NULL)
    {
        new_order->items = NULL;
    }
    else
    {
        assert(order->items != NULL);
        new_order->items = asCopy(order->items);
        if(new_order->items == NULL)
        {
            orFreeOrder(new_order);
            return NULL;
        }
    }
    return new_order;
}

void orFreeOrderVoid(void* order)
{
    orFreeOrder((Order)order);
    return;
}

void orFreeOrder(Order order)
{
    if(order == NULL)
    {
        return;
    }
    assert(order != NULL);
    asDestroy(order->items);
    order->items = NULL;
    free(order);
    return;
}

int orComperOrdersVoid(void* order1, void* order2)
{
    return orComperOrders((Order)order1, (Order)order2);
}

int orComperOrders(Order order1, Order order2)
{
    if(order1 == NULL || order2 == NULL)
    {
        return 0;
    }
    assert(order1 != NULL && order2 != NULL);
    if(order1->id > order2->id)
    {
        return 1;
    }
    if(order1->id < order2->id)
    {
        return -1;
    }
    return 0;
}

void orDeleteProduct(Order order , int id)
{
    if(order == NULL)
    {
        return;
    }

    Items iterator = asGetFirst(order->items);
    while(iterator != NULL)
    {
        if(itGetId(iterator) == id)
        {
            asDelete(order->items, iterator);
            return;
        }
        iterator = asGetNext(order->items);
    }
    return;
}

unsigned int orGetOrderId(Order order)
{
    if(order == NULL)
    {
        return 0;
    }
    return order->id;
}

OrderResult orChangeAmountItem(Order order, const unsigned int item_id , const double amount)
{
    if(order == NULL)
    {
        return ORDER_NULL_ARGUMENT;
    }
    Items iterator = asGetFirst(order->items);
    while(iterator != NULL)
    {
        if(itGetId(iterator) == item_id)
        {   
            if(asChangeAmount(order->items, iterator, amount) == AS_INSUFFICIENT_AMOUNT)
            {
                return ORDER_INSUFFICIENT_AMOUNT;
            }
            return ORDER_SUCCESS;
        }
        iterator = asGetNext(order->items);
    }
    Items item = itCreateItem();
    if(item == NULL)
    {
        return ORDER_OUT_OF_MEMORY;
    }
    itSetItem(item, item_id);
    
    if(asRegister(order->items, item) == AS_OUT_OF_MEMORY)
    {
        itFreeItem(item);  
        return ORDER_OUT_OF_MEMORY;
    }
    asChangeAmount(order->items, item, amount);
    itFreeItem(item);
    return ORDER_SUCCESS;
}

Items orGetFirstItem (Order order){
    if (order == NULL){
        return NULL;
    }
    assert (order!=NULL);
    return asGetFirst(order->items);
}

double orGetAmount (Order order, Items iterator)
{
    double out_amount;
    assert(order->items != NULL);
    assert(iterator != NULL);
    AmountSetResult get_result = asGetAmount(order->items, iterator , &out_amount);
    if(get_result != AS_SUCCESS)
    {
        return 0;
    }
    assert(get_result== AS_SUCCESS);
    return out_amount;
}

Items orGetNextItem (Order order){
    if (order == NULL){
        return NULL;
    }
    assert (order!=NULL);
    return asGetNext(order->items);
}

void orRemoveProductFromOrder(Order order, unsigned int item_id)
{
    if(order == NULL)
    {
        return;
    }
    Items iterator = asGetFirst(order->items);
    while(iterator != NULL)
    {
        if(itGetId(iterator) == item_id)
        {   
            asDelete(order->items, iterator);
            return;
        }
        iterator = asGetNext(order->items);
    }
    return;
}

