#ifndef PRODUCT_H_
#define PRODUCT_H_

#include "matamikya_order.h"
#include "matamikya_items.h"
#include "matamikya.h"
#include "set.h"
#include "amount_set.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


typedef enum ProductResult_t {
    PRODUCT_SUCCESS = 0,
    PRODUCT_NULL_ARGUMENT,
    PRODUCT_OUT_OF_MEMORY,
    PRODUCT_INVALID_NAME,
    PRODUCT_INVALID_AMOUNT,
    PRODUCT_ITEM_ALREADY_EXIST,
    PRODUCT_ITEM_NOT_EXIST,
    PRODUCT_INSUFFICIENT_AMOUNT,
} ProductResult;

/** Type for defining the product */
typedef struct product_t *Product;
/**
 * @brief Create a Product object
 * 
 * @return Product 
 */
Product prCreateProduct();

/**
 * @brief puts the values its given to the product
 * 
 * @param product 
 * @param name 
 * @param id 
 * @param data 
 * @param copyData 
 * @param freeData 
 * @param getProductPrice 
 * @param amount_type 
 * @param income 
 * 
 * @return Product_Sucsses if sucssided
 * product_out_of_memory if feild
 */
ProductResult prGetValuseToProduct(Product product, const char *name, unsigned int id, MtmProductData data, MtmCopyData copyData, 
                                MtmFreeData freeData, MtmGetProductPrice getProductPrice,
                                MatamikyaAmountType amount_type, double income);

/**
 * @brief copy an old_product to a new product
 * 
 * @param old_product 
 * @return Product 
 */
Product prCopyProduct(Product old_product);

/**
 * @brief see prCopyProduct
 * 
 * @param old_product 
 * @return void* 
 */
void* prCopyProductVoid(void* old_product);

/**
 * @brief frees the product element
 * 
 * @param product 
 */
void prFreeProduct(Product product);
/**
 * @brief see prFreeProduct
 * 
 * @param product 
 */
void prFreeProductVoid(void* product);

/**
 * @brief for sentding to ather function
 * for deteils go to prCopyProduct
 * 
 * @param old_product 
 * @return void* 
 */
void* prCopyProductVoid(void* old_product);

/**
 * @brief comper 2 products elements id
 * @param product1 
 * @param product2 
 * @return 1 if product1's id is larger then product2's
 * -1 if product1's id is smaller then product2's
 * 0 if equal
 */
int prProdactCompare(Product product1, Product product2);

/**
 * @brief 
 * 
 * @param product1 
 * @param product2 
 * @return int 
 */
int prProdactCompareVoid(void* product1, void* product2);

/**
 * @brief retuns the products id
 * 
 * @param product 
 * @return ID 
 */
unsigned int prGetProductId(Product product);

/**
 * @brief adds the profit for the product elemnt and retuns the profit
 * 
 * @param product 
 * @param amount 
 * @return double profit 
 */
double prAddAndGetProfit(Product product, double amount);

/**
 * @brief returns the product name
 * 
 */
char* prGetName(Product product);
/**
 * @brief returns the product price for a unit
 * 
 * @param product 
 * @return double 
 */
double prGetPricePerUnit(Product product);

/**
 * @brief retruns the product price for a certain amount
 * 
 * @param product 
 * @param amount 
 * @return double 
 */
double prGetPriceForAmount(Product product, double amount);

/**
 * @brief retrns the profit made from this product
 * 
 * @param product 
 * @return double 
 */
double prGetProductProfit(Product product);

/**
 * @brief returns the amount type
 * 
 * @param product 
 * @return MatamikyaAmountType 
 */
MatamikyaAmountType prGetAmountType(Product product);


#endif