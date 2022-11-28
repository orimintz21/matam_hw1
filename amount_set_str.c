#include "amount_set_str.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct AmountSet_t
{   
    char *name;
    double amount_in_set;
    struct AmountSet_t* next;
};

/**
 * @brief pots the new element in its right place in the arrey
 * needs the set and the new element we want to add
 * @param set
 * @param new_element 
 */
static void setPointer(AmountSet set, AmountSet new_element);
/**
 * @brief Get the Next elemt's name
 * 
 * @param set 
 * @return the next elemnt's name
 */
inline static char* getNextName(AmountSet set);
/**
 * @brief Clear the whole set
 * 
 * @param set 
 * @return 
 */
static void auxClear(AmountSet set);
AmountSet asCreate()
{
    AmountSet set = malloc(sizeof(*set));
    if(!set){
        return NULL;
    }
    set->name = NULL;
    set->amount_in_set = 0;
    set->next = NULL;
    return set;
}

void asDestroy(AmountSet set)
{
    if (set == NULL)
    {
        return;
    }
    
    if (set->next != NULL)
    {
        asDestroy(set->next);
    }

    assert(set!=NULL);
    free(set->name);
    free(set);
    return;
}

AmountSet asCopy(AmountSet set)
{
    if (set == NULL)
    {
        return NULL;
    }

    AmountSet new_set = asCreate();
    if(new_set == NULL)
    {
        return NULL;
    }

    if(set->name == NULL)
    {
        new_set->name = NULL;
    }

    else
    {
        assert(strlen(set->name)>0);
        new_set->name = malloc(sizeof(char)*(strlen(set->name)+1));
        if(new_set->name == NULL)
        {
            free(new_set);
            return NULL;
        }
        strcpy(new_set->name, set->name);
    }

    new_set->amount_in_set = set->amount_in_set;

    if(set->next == NULL)
    {
        new_set->next = NULL;
        return new_set;
    }
    new_set->next = asCopy(set->next);
    if(new_set->next == NULL)
    {
        free(new_set->name);
        free(new_set);
        return NULL;
    }



    return new_set;
}


int asGetSize(AmountSet set)
{
    if (set == NULL)
    {
        return -1;
    }
    assert(set != NULL);

    if(set->next == NULL)
    {
        return 0;
    }
    assert(set->next != NULL);
    return asGetSize(set->next) + 1;
}



bool asContains(AmountSet set, const char* element)
{
    if(set == NULL || element == NULL)
    {
        return false;
    }
    
    AmountSet iterator = set;

    while(iterator->next != NULL)
    {
        assert(iterator->next != NULL);
        iterator = iterator->next;
        if(strcmp(iterator->name, element) == 0)
        {
            return true;       
        }
        if(strcmp(iterator->name, element) > 0)
        {
            return false;
        }
        
        assert(strcmp(iterator->name, element)<0);
        assert(element != NULL);        
    }
    return false;
}


AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount)
{
    if(set == NULL || element == NULL || outAmount == NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element) == false)
    {
        return AS_ITEM_DOES_NOT_EXIST;
    }
    AmountSet iterator = set;
    while (iterator->next != NULL)
    {
        assert(iterator->next != NULL);
        iterator = iterator->next;
        if(strcmp(iterator->name, element) == 0)
        {
            *outAmount = iterator->amount_in_set;
            return AS_SUCCESS;
        }
        if(strcmp(iterator->name, element) > 0)
        {
            return AS_ITEM_DOES_NOT_EXIST;
        }
        
        assert(strcmp(iterator->name, element)<0);
        assert(element != NULL);        
    }

    return AS_ITEM_DOES_NOT_EXIST;
}


AmountSetResult asRegister(AmountSet set, const char* element)
{
    if(set == NULL || element == NULL)
    {
        return AS_NULL_ARGUMENT;
    }

    if(asContains(set,element))
    {
        return AS_ITEM_ALREADY_EXISTS;
    }

    AmountSet new_element = asCreate();
    if(new_element == NULL)
    {
        return AS_OUT_OF_MEMORY;
    }

    new_element->name = malloc((strlen(element)+1)*sizeof(char));
    if(new_element->name == NULL)
    {
        free(new_element);
        return AS_OUT_OF_MEMORY;
    }
    strcpy(new_element->name, element);

    setPointer(set, new_element);
    return AS_SUCCESS;
}

static void setPointer(AmountSet set, AmountSet new_element)
{
    if(set->next == NULL)
    {
        new_element->next = NULL;
        set->next = new_element;
        return;
    }
    AmountSet iterator = set->next;
    if(strcmp(iterator->name, new_element->name)>0)
    {
        set->next = new_element;
        new_element->next = iterator;
        return;
    }
    while (iterator->next != NULL)
    {

        assert(iterator->next != NULL);
        assert(strcmp(getNextName(iterator), new_element->name) != 0);
        assert(getNextName(iterator)!=NULL);
        if(strcmp(getNextName(iterator), new_element->name)>0)
        {
            new_element->next = iterator->next;
            iterator->next = new_element;
            return;
        }
        iterator = iterator->next;
    }
    new_element->next = NULL;
    iterator->next = new_element;
    return;
}

inline static char* getNextName(AmountSet set)
{
    assert(set != NULL);
    if(set->next == NULL)
    {
        return NULL;
    }
    AmountSet next_item = set->next;
    return(next_item->name);
}


AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount)
{
    if (set == NULL || element == NULL)
    {
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element) == false)
    {
        return AS_ITEM_DOES_NOT_EXIST;
    }
    if(set->next == NULL)
    {
        return AS_ITEM_DOES_NOT_EXIST;
    }

    assert(set->next != NULL);
    AmountSet iterator = set->next;     
    while (iterator != NULL)
    {
        if(strcmp(iterator->name, element) == 0)
        {
            if((iterator->amount_in_set + amount) < 0)
            {
               return AS_INSUFFICIENT_AMOUNT;
            }
            iterator->amount_in_set += amount;
            return AS_SUCCESS; 
        }
        
        assert((strcmp(iterator->name, element)) < 0);
        iterator = iterator->next;       
    }

    return AS_ITEM_DOES_NOT_EXIST;
}


AmountSetResult asDelete(AmountSet set, const char* element){
    if (set == NULL || element == NULL){
        return  AS_NULL_ARGUMENT;
    }
    
    if(!(asContains(set,element)))
    {
        return AS_ITEM_DOES_NOT_EXIST;
    }
    AmountSet iterator = set; 
    
    while (iterator != NULL)
    {

        if(iterator->next == NULL)
        {
            return AS_ITEM_DOES_NOT_EXIST;
        }

        assert(iterator->next != NULL);
        assert(getNextName(iterator)!=NULL);

        if(strcmp(getNextName(iterator),element) == 0)
        {
            AmountSet temp_item = iterator->next;
            iterator->next = temp_item->next;
            free(temp_item->name);
            free(temp_item);
            return AS_SUCCESS;
        }
        iterator = iterator->next;
    }
    
    return AS_ITEM_DOES_NOT_EXIST;
}

AmountSetResult asClear(AmountSet set){
    if (set == NULL){
        return  AS_NULL_ARGUMENT;
    }
    assert(set != NULL);
    if(set->next != NULL)
    {
        auxClear(set->next);
    }
    char* temp_name = set->name;
    free(temp_name);
    set->name = NULL;
    set->next = NULL;
    return AS_SUCCESS;
}

static void auxClear(AmountSet set){
      if(set->next != NULL)
    {
        auxClear(set->next);
    }
        char* temp_name =set->name;
        free(temp_name);
        set->name =NULL;
        
        AmountSet temp_set = set;
        free(temp_set);
        set = NULL;
        return;
}


char* asGetFirst(AmountSet set){
    if (set == NULL || set->next == NULL ){
        return  NULL;
    }
    assert(set != NULL);
    assert(set->next != NULL);
    AmountSet first_item = set->next;
    char* temp = set->name;
    free(temp);
    set->name = malloc(sizeof(char)*(strlen(first_item->name)+1));
    if(set->name == NULL)
    {
        printf("eror: Set iterator status is undifined");
        return first_item->name;
    }
    strcpy(set->name, first_item->name);
    return first_item->name; 
}



char* asGetNext(AmountSet set)
{
    if(set == NULL || set->name == NULL || set->next == NULL)
    {
        return NULL;
    }

    AmountSet iterator = set->next;
    while (iterator != NULL)
    {
        if(strcmp(iterator->name , set->name) == 0)
        {
            assert(set->name != NULL);

            if(getNextName(iterator) == NULL)
            {
                free(set->name);
                set->name = NULL;
                return NULL;
            }
            assert(getNextName(iterator)!=NULL);
            char* temp_name = set->name;
            free(temp_name);
            set->name = NULL;
            set->name = malloc(sizeof(char)* (strlen(iterator->name)+1));
            if(set->name == NULL)
            {
                printf("malloc faild, iterator is undified");
                return getNextName(iterator);
            }
            strcpy(set->name,getNextName(iterator));
            return getNextName(iterator);
        }

        iterator = iterator->next;
    }
    return NULL;   
}
