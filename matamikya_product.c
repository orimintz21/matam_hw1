#include "matamikya.h"
#include "amount_set.h"
#include "set.h"
#include "matamikya_product.h"
#include "matamikya_items.h"
#include "matamikya_order.h"
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

struct product_t
{
    char *name;
    unsigned int id;
    MtmProductData data;
    MtmCopyData copyData;
    MtmFreeData freeData;
    MtmGetProductPrice getProductPrice;
    MatamikyaAmountType amount_type;
    double income;
};


Product prCreateProduct()
{
    Product new_product= malloc(sizeof(*new_product));
    if(new_product == NULL)
    {
        return NULL;
    }

    new_product->name = NULL;
    new_product->copyData = NULL;
    new_product->freeData = NULL;
    new_product->data = NULL;
    new_product->amount_type = MATAMIKYA_INTEGER_AMOUNT;
    new_product->income = 0;
    return new_product;
}

ProductResult prGetValuseToProduct(Product product, const char *name, unsigned int id,
                                MtmProductData data, MtmCopyData copyData, 
                                MtmFreeData freeData,MtmGetProductPrice getProductPrice ,
                                MatamikyaAmountType amount_type, double income)
{
    if(product == NULL || name == NULL || data == NULL || copyData == NULL || 
        freeData == NULL|| getProductPrice ==NULL)
    {
        return PRODUCT_NULL_ARGUMENT;
    }
    product->name = malloc(sizeof(char)*(strlen(name) + 1));
    if(product->name == NULL)
    {
        return PRODUCT_OUT_OF_MEMORY;
    }
    strcpy(product->name,name);
    product->id = id;
    product->copyData = copyData;
    product->freeData = freeData;
    product->getProductPrice = getProductPrice;
    product->data = product->copyData(data);
    product->amount_type = amount_type;
    product->income = income;

    return PRODUCT_SUCCESS;
}

void* prCopyProductVoid(void* old_product)
{
    return (void*)prCopyProduct((Product)old_product);
}


Product prCopyProduct(Product old_product)
{
    
    Product new_product = prCreateProduct();
    if(new_product == NULL)
    {
        return NULL;
    }
    if(old_product->name != NULL)
    {
        char* new_name = malloc(sizeof(char)*(strlen(old_product->name)+1));
        if(new_name == NULL)
        {   
            prFreeProduct(new_product);
            return NULL;
        }
        new_product->name = new_name;        
        strcpy(new_product->name, old_product->name);
    }
    
    else
    {
        new_product->name = NULL;
    }
    
    assert(old_product->copyData != NULL);
    new_product->data = old_product->copyData(old_product->data);
    new_product->id = old_product->id;
    new_product->freeData = old_product->freeData;
    new_product->copyData = old_product->copyData;
    new_product->getProductPrice = old_product->getProductPrice;
    new_product->amount_type = old_product->amount_type;   
    new_product->income = old_product->income;
    return new_product;
}

void prFreeProductVoid(void* product)
{
    prFreeProduct((Product)product);
    return;
}


void prFreeProduct(Product product)
{
    if(product == NULL)
    {
        return;
    }
    free(product->name);
    product->freeData(product->data);
    product->freeData = NULL;
    product->copyData = NULL;
    product->getProductPrice = NULL;
    free(product);
    product = NULL;
    return;
}

int prProdactCompareVoid(void* product1, void* product2)
{
    return prProdactCompare((Product) product1, (Product) product2);
}

int prProdactCompare(Product product1, Product product2)
{
    if((product1->id) > (product2->id))
    {
        return 1;
    }
    
    if((product1->id) < (product2->id))
    {
        return -1;
    }
    return 0;
}

double prGetId(Product product)
{
    if(product == NULL)
    {
        return 0;
    }
    return product->id;
}

char* prGetName(Product product)
{
    if(product == NULL)
    {
        return NULL;
    }
    return product->name;
}

double prGetPricePerUnit(Product product)
{
    if(product == NULL)
    {
        return 0;
    }
    return product->getProductPrice(product->data,1);
}

double prGetPriceForAmount(Product product, double amount)
{
    if(product == NULL)
    {
        return 0;
    }
    return product->getProductPrice(product->data,amount);
}

double prAddAndGetProfit(Product product, double amount)
{
    double profit = product->getProductPrice(product->data, amount);
    assert(profit>0);
    product->income += profit;
    return profit;
}

double prGetProductProfit(Product product)
{
    if(product == NULL)
    {
        return 0;
    }
    return product->income;
}

MatamikyaAmountType prGetAmountType(Product product)
{
    if(product == NULL)
    {
        return MATAMIKYA_INTEGER_AMOUNT;
    }
    return product->amount_type;
}


unsigned int prGetProductId(Product product)
{
    if(product == NULL)
    {
        return 0;
    }
    return product->id;
}
