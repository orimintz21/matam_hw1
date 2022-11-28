#ifndef ORDER_H_
#define ORDER_H_

#include "matamikya_product.h"
#include "matamikya.h"
#include "set.h"
#include "matamikya_items.h"
#include "amount_set.h"
#include "matamikya_print.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>



/** Element data type for order container */
typedef struct Order_t *Order;
/**
 * @brief creates new order
 * 
 * @return Order 
 */
Order orCreatOrder();


/**
 * @brief copy new order
 * 
 * @param old_order 
 * @return SetElement 
 */
SetElement orCopyOrderVoid(SetElement old_order);


/** Type of function for deallocating an element of the order */
void orFreeOrder(Order order);

/**
 * @brief see orFreeOrder
 * 
 * @param order 
 */
void orFreeOrderVoid(void* order);



typedef enum OrderResult_t {
    ORDER_SUCCESS = 0,
    ORDER_NULL_ARGUMENT,
    ORDER_OUT_OF_MEMORY,
    ORDER_ITEM_NOT_EXIST,
    ORDER_INSUFFICIENT_AMOUNT,

} OrderResult;

/**
 * @brief 
 * 
 * @param order1 
 * @param order2 
 * @return 1 if order1's id is larger then order2's
 * -1 if order1's id is smaller then order2's
 * 0 else
 */
int orComperOrders(Order order1, Order order2);

/**
 * @brief see orComperOrders
 * 
 * @param order1 
 * @param order2 
 * @return int 
 */
int orComperOrdersVoid(void* order1, void* order2);



/**
 * @brief pots order id in order
 * 
 * @param order 
 * @param id 
 */
void orGetIdToOrder(Order order, unsigned int id);
/**
 * @brief
 * 
 * @param order 
 * @return unsigned int - return the id of this order 
 */
unsigned int orGetOrderId(Order order);

/**
 * @brief if the item is in the order, the function delets it
 * 
 * @param order 
 * @param id 
 */
void orDeleteProduct(Order order , const int id);

/**
 * @brief return the first item in the order
 * 
 * @param order 
 * @return NULL if the order is null or the  NULL pointer was sent or the item set is empty.
 *     The first element of the item set otherwise
 */
Items orGetFirstItem (Order order);

/**
 * @brief return the amoutof the iterator in the order
 * 
 * @param order 
  * @param iterator 
 * @return the amout of the iterator item in the order
 *     
 */
double orGetAmount (Order order, Items iterator);

/**
 * @brief return the next item in the order
 *
 * @param set - The set for which to advance the iterator
 * @return
 *     NULL if reached the end of the items set, or the iterator is at an invalid state
 *     The next element on the set in case of success
 */
 Items orGetNextItem (Order order);
/**
 * @brief change the amount of the item
 * 
 * @param order 
 * @param item_id 
 * @param amount 
 * @return OrderResult 
 */
 OrderResult orChangeAmountItem(Order order, const unsigned int item_id , const double amount);

/**
 * @brief removes item for order if its exist
 * 
 */
void orRemoveProductFromOrder(Order order, unsigned int item_id);


#endif