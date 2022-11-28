#include "matamikya.h"
#include "amount_set.h"
#include "set.h"
#include "matamikya_order.h"
#include "matamikya_product.h"
#include "matamikya_items.h"
#include "matamikya_print.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/**
 * @brief checks if the amount is whithin 0.001 from its expected value
 * 
 * @param amount 
 * @param amountType 
 * @return MatamikyaResult 
 */
static MatamikyaResult mtmVerifyNewProductAmount(double amount, const MatamikyaAmountType amount_type);

/**
 * @brief checks if the product is in the warehouse and if it is checks if the amount type is right whith amount 
 * 
 * @param products 
 * @param id 
 * @return MatamikyaResult 
 */
static MatamikyaResult mtmFindIfProductIsInWarehouseAndAmount(AmountSet products, unsigned int id, double amount);

/**
 * @brief if the order exists in the orders lits the function will return it
 * else it will return null pointer 
 * 
 * @param orders 
 * @param id 
 * @return Order 
 */
static Order mtmGetOrder(Set orders , unsigned int id);

/**
 * @brief verify that all the aruments are valid
 * 
 * @param matamikya 
 * @param id 
 * @param name 
 * @param amount 
 * @param amountType 
 * @param customData 
 * @param copyData 
 * @param freeData 
 * @param prodPrice 
 * @return MatamikyaResult 
 */
static MatamikyaResult mtmVerifyNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice);


/**
 * @brief delets the product if its exist in the products list
 * 
 * @param products 
 * @param id 
 * @return if its exist returns MTM_SUCSSES
 * if gets NULL pointer returns MATAMIKYA_NULL_ARGUMENT
 * if the item isn't in the set MATAMIKYA_PRODUCT_NOT_EXIST
 */
static MatamikyaResult mtmClearProductWarehouse(AmountSet products, unsigned int id);

/**
 * @brief ships the products that are in the order
 * adds the profit to the warehouse and the product elemt
 * 
 * doesn't verify anything beforhend!
 * doesn't free the order!
 * @param matamikya 
 * @param order 
 */
static void mtmShipVerifyOrder(Matamikya matamikya, Order order);

/**
 * @brief delets this prodact from all the orders
 * 
 * @param orders 
 * @param id - of the product
 */
static void mtmClearProductOrders(Set orders , unsigned int id);
/**
 * @brief finds if the order amount is larger then the item amount for all the items in the order
 * if not the function will return MATAMIKYA_INSUFFICIENT_AMOUNT
 * 
 * 
 * @param matamikya 
 * @param order 
 * @return MatamikyaResult 
 */
static MatamikyaResult mtmFIndIfProductsAmountIsLargerThenItemAmount
                                    (Matamikya matamikya, Order order);

struct Matamikya_t
{
    AmountSet products;
    Set orders;
    double profit;
    unsigned int largest_order_number;
};

Matamikya matamikyaCreate()
{
    Matamikya warehouse = malloc(sizeof(*warehouse));
    if(warehouse == NULL)
    {
        return NULL;
    }
    warehouse->products = asCreate(prCopyProductVoid, prFreeProductVoid, prProdactCompareVoid);
    if(warehouse->products == NULL)
    {
        free(warehouse);
        return NULL;
    }
    warehouse->orders = setCreate(orCopyOrderVoid, orFreeOrderVoid,orComperOrdersVoid);
    if(warehouse->orders == NULL)
    {
        asDestroy(warehouse->products);
        free(warehouse);
        return NULL;
    }
    warehouse->profit = 0;
    warehouse->largest_order_number = 0;
    return warehouse;
}

void matamikyaDestroy(Matamikya matamikya)
{
    if(matamikya == NULL)
    {
        return;
    }
    asDestroy(matamikya->products);
    matamikya->products = NULL;
    setDestroy(matamikya->orders);
    matamikya->orders = NULL;
    free(matamikya);
    matamikya = NULL;
    return;
}

static MatamikyaResult mtmVerifyNewProductAmount(double amount, const MatamikyaAmountType amount_type)
{
    if(amount_type == MATAMIKYA_ANY_AMOUNT)
    {
        return MATAMIKYA_SUCCESS;
    }

    if(amount_type == MATAMIKYA_INTEGER_AMOUNT)
    {
        double distance = amount*1000-(round(amount))*1000;
        if(distance <= 1 && distance>= -1)
        {
            return MATAMIKYA_SUCCESS;
        }
        return MATAMIKYA_INVALID_AMOUNT;
    }
    
    assert(amount_type == MATAMIKYA_HALF_INTEGER_AMOUNT);
    
    double distance = amount*2000-(round(amount*2))*1000;
    if(distance <= 2 && distance>=-2)
    {
        return MATAMIKYA_SUCCESS;
    }

    return MATAMIKYA_INVALID_AMOUNT;
}

static MatamikyaResult mtmVerifyNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    if(matamikya == NULL || name == NULL || customData == NULL ||
                             freeData == NULL || prodPrice == NULL || copyData == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    if(!((name[0]>='a'&&name[0]<='z')||(name[0]>='A'&&name[0]<='Z')||(name[0]>='0'&&name[0]<='9')))
    {
        return MATAMIKYA_INVALID_NAME;
    }

    if(amount < 0)
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }

    if(mtmVerifyNewProductAmount(amount, amountType) == MATAMIKYA_INVALID_AMOUNT)
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    MatamikyaResult test = mtmVerifyNewProduct(matamikya,id,name,amount,amountType,
                                                customData,copyData,freeData,prodPrice);
    if(test != MATAMIKYA_SUCCESS)
    {
        return test;
    }
    Product new_product = prCreateProduct();
    if(new_product == NULL)
    {
        return MATAMIKYA_OUT_OF_MEMORY;   
    }
    ProductResult get_valuse_result = prGetValuseToProduct(new_product, name, id, customData, copyData, 
                                                            freeData, prodPrice, amountType, 0);
    if(get_valuse_result == PRODUCT_OUT_OF_MEMORY)
    {
        prFreeProduct(new_product);
        return MATAMIKYA_OUT_OF_MEMORY;
    }

    AmountSetResult register_status = asRegister(matamikya->products, new_product);
    if(register_status == AS_ITEM_ALREADY_EXISTS)
    {
        prFreeProduct(new_product);
        return MATAMIKYA_PRODUCT_ALREADY_EXIST;
    }
    asChangeAmount(matamikya->products, new_product, amount);
    prFreeProduct(new_product);
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount)
{
    if(matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    
    Product iterator;
    iterator = asGetFirst(matamikya->products);
    while(iterator != NULL)
    {
        assert(iterator != NULL);
        if(prGetProductId(iterator) == id)
        {
            MatamikyaAmountType product_type = prGetAmountType(iterator);
            MatamikyaResult amount_type_check = mtmVerifyNewProductAmount(amount, product_type);
            if(amount_type_check != MATAMIKYA_SUCCESS)
            {
                return MATAMIKYA_INVALID_AMOUNT;
            }
            AmountSetResult change_result = asChangeAmount(matamikya->products, iterator, amount);
            if(change_result == AS_INSUFFICIENT_AMOUNT)
            {
                return MATAMIKYA_INSUFFICIENT_AMOUNT;
            }
            return MATAMIKYA_SUCCESS;
        }
        iterator = asGetNext(matamikya->products);
    }
    return MATAMIKYA_PRODUCT_NOT_EXIST;
}

static MatamikyaResult mtmClearProductWarehouse(AmountSet products, unsigned int id)
{
    if(products == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    
    Product iterator;
    iterator = asGetFirst(products);
    while(iterator != NULL)
    {
        assert(iterator != NULL);
        if(prGetProductId(iterator) == id)
        {
            asDelete(products, iterator);
            return MATAMIKYA_SUCCESS;
        }
        iterator = asGetNext(products);
    }
    return MATAMIKYA_PRODUCT_NOT_EXIST;   
}

static MatamikyaResult mtmFindIfProductIsInWarehouseAndAmount
                                (AmountSet products, unsigned int id, double amount)
{
    if(products == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    
    Product iterator;
    iterator = asGetFirst(products);
    while(iterator != NULL)
    {
        assert(iterator != NULL);
        if(prGetProductId(iterator) == id)
        {
            MatamikyaAmountType amount_type = prGetAmountType(iterator);
            if(mtmVerifyNewProductAmount(amount , amount_type) == MATAMIKYA_INVALID_AMOUNT)
            {
                return MATAMIKYA_INVALID_AMOUNT;
            }
            return MATAMIKYA_SUCCESS;
        }
        iterator = asGetNext(products);
    }
    return MATAMIKYA_PRODUCT_NOT_EXIST;   
}

static void mtmClearProductOrders(Set orders , unsigned int id)
{
    if(orders == NULL)
    {
        return;
    }
    Order iterator = setGetFirst(orders);
    while(iterator != NULL)
    {
        assert(iterator!= NULL);
        orDeleteProduct(iterator, id);
        iterator = setGetNext(orders);
    }
    return;
}

static Order mtmGetOrder(Set orders , unsigned int id)
{
    if(orders == NULL)
    {
        return NULL;
    }
    Order iterator = setGetFirst(orders);
    while(iterator != NULL)
    {
        assert(iterator!= NULL);
        if(orGetOrderId(iterator) == id)
        {
            return iterator;
        }
        iterator = setGetNext(orders);
    }
    return NULL;
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id)
{
    if(matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    assert(matamikya != NULL);
    MatamikyaResult clear_product_in_warehouse_result = mtmClearProductWarehouse(matamikya->products, id);
    if(clear_product_in_warehouse_result != MATAMIKYA_SUCCESS)
    {
        return clear_product_in_warehouse_result;
    }

    mtmClearProductOrders(matamikya->orders , id);
    return MATAMIKYA_SUCCESS;
}


unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
    if(matamikya == NULL)
    {
        return 0;
    }
    Order new_order = orCreatOrder();
    if(new_order == NULL)
    {
        return 0;
    }

    unsigned int order_id = matamikya->largest_order_number + 1;
    orGetIdToOrder(new_order, order_id);
    SetResult set_add = setAdd(matamikya->orders, new_order);
    assert(set_add != SET_ITEM_ALREADY_EXISTS);

    if(set_add == SET_OUT_OF_MEMORY)
    {
        orFreeOrder(new_order);
        return 0;
    }
    matamikya->largest_order_number = matamikya->largest_order_number + 1;
    orFreeOrder(new_order);
    return order_id;
}

MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
                                     const unsigned int productId, const double amount)
{
    if(matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    MatamikyaResult amount_in_warehouse_and_amount_type = 
                        mtmFindIfProductIsInWarehouseAndAmount(matamikya->products, productId, amount);

    if(amount_in_warehouse_and_amount_type != MATAMIKYA_SUCCESS)
    {
        return amount_in_warehouse_and_amount_type;
    }

    Order iterator = setGetFirst(matamikya->orders);
    while(iterator != NULL)
    {
        if(orGetOrderId(iterator) == orderId)
        {
            if(amount == 0)
            {
                return MATAMIKYA_SUCCESS;
            }

            OrderResult change_amount_result = orChangeAmountItem(iterator, productId, amount);
            if(change_amount_result == ORDER_SUCCESS)
            {
                return MATAMIKYA_SUCCESS;
            }
            if(change_amount_result == ORDER_OUT_OF_MEMORY)
            {
                return MATAMIKYA_OUT_OF_MEMORY;
            }
            if(change_amount_result == ORDER_INSUFFICIENT_AMOUNT)
            {
                orRemoveProductFromOrder(iterator, productId);
                return MATAMIKYA_SUCCESS;
            }
        }
        iterator = setGetNext(matamikya->orders);
    }
    return MATAMIKYA_ORDER_NOT_EXIST;
}

static Product mtmGetProduct(AmountSet products, unsigned int id)
{
    if(products == NULL)
    {
        return NULL;
    }
    Product iterator = asGetFirst(products);
    while(iterator != NULL)
    {
        if(prGetProductId(iterator) == id)
        {
            return iterator;
        }
        iterator = asGetNext(products);
    }
    return NULL;
}

static MatamikyaResult mtmFIndIfProductsAmountIsLargerThenItemAmount
                                    (Matamikya matamikya, Order order)
{
    Items item = orGetFirstItem(order);
    while(item != NULL)
    {
        unsigned int item_id = itGetId(item);
        Product product = mtmGetProduct(matamikya->products, item_id);
        if(product == NULL)
        {
            return MATAMIKYA_NULL_ARGUMENT;
        }
        double product_amount;
        double item_amount = orGetAmount ( order, item);
        asGetAmount(matamikya->products, product, &product_amount);
        if(product_amount < item_amount)
        {
            return MATAMIKYA_INSUFFICIENT_AMOUNT;
        }
        item = orGetNextItem(order);
    }
    return MATAMIKYA_SUCCESS;
}

static void mtmShipVerifyOrder(Matamikya matamikya, Order order)
{
    Items item = orGetFirstItem(order);
    while(item != NULL)
    {
        unsigned int item_id = itGetId(item);
        Product product = mtmGetProduct(matamikya->products, item_id);
        assert(product != NULL);

        double product_amount;
        double item_amount = orGetAmount (order, item);
        asGetAmount(matamikya->products, product, &product_amount);
        assert(product_amount >= item_amount);
    
        asChangeAmount(matamikya->products, product, -1*item_amount);
   //     asGetAmount(matamikya->products, product, &product_amount);

        double profit = prAddAndGetProfit(product, item_amount);
        matamikya->profit += profit;  
        
        item =orGetNextItem(order);
    }
    return;
}

MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId)
{
    if(matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order order = mtmGetOrder(matamikya->orders, orderId);
    if(order == NULL)
    {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    MatamikyaResult can_ship = mtmFIndIfProductsAmountIsLargerThenItemAmount(matamikya, order);
    if(can_ship != MATAMIKYA_SUCCESS)
    {
        return can_ship;
    }
    mtmShipVerifyOrder(matamikya, order);
    
    setRemove(matamikya->orders, order);

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId)
{
    if(matamikya == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    Order order = mtmGetOrder(matamikya->orders, orderId);
    if(order == NULL)
    {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    setRemove(matamikya->orders, order);

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output)
{
    if(matamikya == NULL || output == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    fprintf(output, "Inventory Status:\n");
    Product product = asGetFirst(matamikya->products);
    while(product != NULL)
    {
        assert(product!=NULL);
        double amount;
        double price = prGetPricePerUnit(product);
        asGetAmount(matamikya->products, product, &amount);
        mtmPrintProductDetails(prGetName(product), prGetProductId(product), amount, price, output);
        product = asGetNext(matamikya->products);
    }
    return MATAMIKYA_SUCCESS;
}


MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE *output)
{
     if(matamikya == NULL || output == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }   
    if (mtmGetOrder( matamikya->orders , orderId) == NULL){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    double sum_price=0;
    mtmPrintOrderHeading(orderId,  output);
    Order order = mtmGetOrder(matamikya->orders, orderId);
    assert(order!=NULL);
    Items iterator = orGetFirstItem(order); 
    while(iterator != NULL)
    {   
        Product curr_item = mtmGetProduct( matamikya->products,  itGetId(iterator));
        double amount = orGetAmount(order,  iterator);
        double item_price = prGetPriceForAmount(curr_item, amount);
        mtmPrintProductDetails(prGetName(curr_item), prGetProductId(curr_item),  amount, item_price, output);
        sum_price+= item_price;
        iterator = orGetNextItem (order);
    }
    mtmPrintOrderSummary(sum_price, output);

    return MATAMIKYA_SUCCESS;
    
}



MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output)
{
    if(matamikya == NULL || output == NULL)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }
    fprintf(output, "Best Selling Product:\n");
    if(matamikya->profit == 0)
    {
        fprintf(output, "none\n");
        return MATAMIKYA_SUCCESS;
    }
    if(matamikya->products == NULL)
    {
        fprintf(output, "none\n");
        return MATAMIKYA_SUCCESS;
    }
    
    Product best_selling = asGetFirst(matamikya->products);
    double best_selling_profit =  prGetProductProfit(best_selling);
    Product iterator = best_selling;
    
    while (iterator!=NULL)
    {
        if(prGetProductProfit(iterator) > best_selling_profit)
        {
            best_selling_profit = prGetProductProfit(iterator);
            best_selling = iterator;
        }
        iterator = asGetNext(matamikya->products);
    }
    mtmPrintIncomeLine(prGetName(best_selling), prGetProductId(best_selling),
                                    prGetProductProfit(best_selling), output);
    
    return MATAMIKYA_SUCCESS;
}