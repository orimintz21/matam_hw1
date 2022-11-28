#include "amount_set_str.h"
#include "amount_set_str_tests.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/**
 * @brief register and set amount for all the elemnts for the test
 * 
 * @param set 
 * @return TestAmountSetResult 
 */
static TestAmountSetResult asRegisterWithAmount(AmountSet set);

/**
 * @brief register all the elemnts for the test
 * 
 * @param set 
 * @return TestAmountSetResult 
 */
static TestAmountSetResult asContainsTestRegister(AmountSet set);

/**
 * @brief test all the null options
 * 
 * @param set 
 * @return TestAmountSetResult 
 */
static TestAmountSetResult asRegisterTestNull (AmountSet set);
/**
 * @brief  test change amout with null arg
 * 
 * @return TestAmountSetResult 
 */
static TestAmountSetResult asChangeAmountTestNull();
/**
 * @brief test asDelete with null arg
 * 
 * @return TestAmountSetResult 
 */
static TestAmountSetResult asDeleteTestNull(AmountSet set);

TestAmountSetResult asCreatTest()
{
    AmountSet new_set = asCreate();
    if(new_set == NULL)
    {
        return TEST_FAILD;
    }
    asDestroy(new_set);
    return TEST_SUCCESS;
}

TestAmountSetResult asCopyTestNull()
{
    if(asCopy(NULL) == NULL)
    {
        return TEST_SUCCESS;
    }
    return TEST_FAILD;
}

TestAmountSetResult asCopyTestEmpty()
{
    AmountSet empty_set = asCreate();
    if(empty_set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }
    AmountSet copy_set = asCopy(empty_set);
    if(copy_set == NULL)
    {
        return TEST_FAILD;
    }
    asDestroy(empty_set);
    asDestroy(copy_set);
    return TEST_SUCCESS;
}

TestAmountSetResult asCopyTest()
{
    AmountSet set = asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }

    asRegister(set, "abcd");
    asGetFirst(set);
    asRegister(set, "tuv");
    asRegister(set, "xyz");
    asChangeAmount(set, "abcd", 5.2);
    asChangeAmount(set, "tuv", 9000000);
    asChangeAmount(set, "xyz", 3);

    AmountSet new_set = asCopy(set);
    if(new_set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }

    char* iterator = malloc(sizeof(char)*(strlen("abcd")+1));
    if(iterator == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }

    strcpy(iterator, asGetFirst(set));
    if(iterator == NULL)
    {
        printf("asGetFirst faild");
        return TEST_FAILD;
    }
    if(strcmp(iterator, asGetFirst(new_set))!= 0)
    {
        printf("copy faild\n");
        return TEST_FAILD;
    }

    if(asGetSize(set) != asGetSize(new_set))
    {
        printf("not the same size\n");
        return TEST_FAILD;
    }
    int i= asGetSize(set) - 1;
    while(i)
    {
        strcpy(iterator, asGetNext(set));
        if(strcmp(iterator, asGetNext(new_set))!=0)
        {
            printf("copy faild\n");
            return TEST_FAILD;
        }      
        i--;       
    }
    free(iterator);
    asDestroy(set);
    asDestroy(new_set);
    return TEST_SUCCESS;
}

TestAmountSetResult asGetSizeTestNull()
{
    if(asGetSize(NULL) != -1)
    {
        printf("asGetSize whith null pointer faild");
        return TEST_FAILD;
    }
    return TEST_SUCCESS;
}

TestAmountSetResult asGetSizeTestEmptySet()
{
    AmountSet set = asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }
    if(asGetSize(set) != 0)
    {
        asDestroy(set);
        printf("asGetSize whith an empty set faild");
        return TEST_FAILD;
    }
    
    asDestroy(set);
    return TEST_SUCCESS;
}

TestAmountSetResult asGetSizeTestFull()
{
    AmountSet set=asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }
    asRegister(set, "ADSFFD1");
    asRegister(set, "GWRGWR2");
    asRegister(set, "grwjio3");
    asRegister(set, "OPOG4");
    asRegister(set, "IOIIOI5");
    asRegister(set, "rgwoij$@%6");
    asRegister(set, "!@#d^7");
    asRegister(set, "jkljjklvf8");
    asRegister(set, "1qsdg9");
    asRegister(set, "fdsajirwnj10");

    if(asGetSize(set) != 10)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    asDestroy(set);
    return TEST_SUCCESS;
}

TestAmountSetResult asContainsTestNullSetOrElement()
{
    if(asContains(NULL,NULL) != false)
    {
        printf("set = null & element = null faild\n");
        return TEST_FAILD;
    }
    
    if(asContains(NULL, "\0") != false)
    {
        printf("set = null faild\n");
        return TEST_FAILD;
    }
    AmountSet set = asCreate();
    asRegister(set, "abc");
    if(asContains(set, NULL) != false)
    {
        printf("element = null faild\n");
        asDestroy(set);
        return TEST_FAILD;
    }

    asDestroy(set);
    return TEST_SUCCESS;
}

static TestAmountSetResult asContainsTestRegister(AmountSet set)
{
    if(asRegister(set, "abcd1")==AS_OUT_OF_MEMORY)
    {
        return TEST_OUT_OF_MEMORY;
    }
    if(asRegister(set, "0000dfs2")==AS_OUT_OF_MEMORY)
    {
        return TEST_OUT_OF_MEMORY;
    }
    if(asRegister(set, "fbrqD3")==AS_OUT_OF_MEMORY)
    {
        return TEST_OUT_OF_MEMORY;
    }
    if(asRegister(set, "!@RFu9o4")==AS_OUT_OF_MEMORY)
    {
        return TEST_OUT_OF_MEMORY;
    }
    if(asRegister(set, "2huireoiu5")==AS_OUT_OF_MEMORY)
    {
        return TEST_OUT_OF_MEMORY;
    }
    return TEST_SUCCESS;
}

TestAmountSetResult asContainsTestFull()
{
    AmountSet set = asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }
    if(asContainsTestRegister(set) == TEST_OUT_OF_MEMORY)
    {
        asDestroy(set);
        return TEST_OUT_OF_MEMORY;
    }
    if(asContains(set,"qwr") == true)
    {
        asDestroy(set);
        printf("return true for a string that isn't contiened in the set\n");
        return TEST_FAILD;
    }
    if(asContains(set,"!@RFu9o4") == false)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    if(asContains(set,"2huireoiu5") == false)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    asDelete(set,"abcd1");
    if(asContains(set,"abcd1") == true)
    {
        asDestroy(set);
        printf("found an element after delete\n");
        return TEST_FAILD;
    }
    asDestroy(set);
    return TEST_SUCCESS;
}

static TestAmountSetResult asRegisterWithAmount(AmountSet set)
{
    if(asRegister(set, "abcd1")==AS_OUT_OF_MEMORY)
    {
        return TEST_OUT_OF_MEMORY;
    }
    asChangeAmount(set, "abcd1", 314.1);
    
    if(asRegister(set, "0000dfs2")==AS_OUT_OF_MEMORY)
    {
        return TEST_OUT_OF_MEMORY;
    }
    asChangeAmount(set, "0000dfs2", 0);
    
    if(asRegister(set, "fbrqD3")==AS_OUT_OF_MEMORY)
    {
        return TEST_OUT_OF_MEMORY;
    }

    if(asRegister(set, "!@RFu9o4")==AS_OUT_OF_MEMORY)
    {
        return TEST_OUT_OF_MEMORY;
    }
    asChangeAmount(set, "!@RFu9o4", 9000000000);

    if(asRegister(set, "2huireoiu5")==AS_OUT_OF_MEMORY)
    {
        return TEST_OUT_OF_MEMORY;
    }
    asChangeAmount(set, "2huireoiu5", 0.00001);

    return TEST_SUCCESS;
}

TestAmountSetResult asGetAmountTestFull()
{
    double amount = 0;
    AmountSet set = asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }

    if(asRegisterWithAmount(set) == TEST_OUT_OF_MEMORY)
    {
        asDestroy(set);
        return TEST_OUT_OF_MEMORY;
    }

 
    if(asGetAmount(NULL,NULL , &amount) != AS_NULL_ARGUMENT)
    {
        printf("set = null & element = null faild\n");
        return TEST_FAILD;
    }
    
    if(asGetAmount(NULL,"abc", &amount) != AS_NULL_ARGUMENT)
    {
        printf("set = null faild\n");
        return TEST_FAILD;
    }

    if(asGetAmount(set, NULL, &amount) != AS_NULL_ARGUMENT)
    {
        printf("element = null faild\n");
        return TEST_FAILD;
    }

    if(asGetAmount(set,"qwr", &amount) != AS_ITEM_DOES_NOT_EXIST)
    {
        printf("return true for a string that isn't contiened in the set\n");
        return TEST_FAILD;
    }

    asGetAmount(set,"!@RFu9o4", &amount);

    if(amount != 9000000000)
    {
        return TEST_FAILD;
    }
    
    asGetAmount(set,"0000dfs2", &amount);
    if(amount != 0)
    {
        return TEST_FAILD;
    }

    asDelete(set,"abcd1");
    if(asGetAmount(set,"abcd1", &amount) != AS_ITEM_DOES_NOT_EXIST)
    {
        printf("found an element after delete\n");
        return TEST_FAILD;
    }

    asDestroy(set);
    return TEST_SUCCESS;
}

static TestAmountSetResult asRegisterTestNull(AmountSet set)
{
    if(asRegister(NULL, NULL) != AS_NULL_ARGUMENT)
    {
        printf("null test faild\n");
        return TEST_FAILD;
    }

    if(asRegister(set, NULL) != AS_NULL_ARGUMENT)
    {
        printf("null test faild\n");
        return TEST_FAILD;
    }

    if(asRegister(NULL, "abcd") != AS_NULL_ARGUMENT)
    {
        printf("null test faild\n");
        return TEST_FAILD;
    }
    return TEST_SUCCESS;

}

TestAmountSetResult asRegisterTest()
{
    AmountSet set = asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }

    if(asRegisterTestNull(set) == TEST_FAILD)
    {
        asDestroy(set);
        return TEST_FAILD;
    }

    if(asRegister(set, "abc") != AS_SUCCESS)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    if(asRegister(set,"abc") != AS_ITEM_ALREADY_EXISTS)
    {
        printf("same set test failed\n");
        asDestroy(set);
        return TEST_FAILD;
    }

    asRegister(set, "def");
    asRegister(set, "gih");
    asRegister(set , "ABC");
    asRegister(set, "!@#$");
    char* iterator_1 = NULL;
    char* iterator_2 = NULL;
    
    iterator_1 = asGetFirst(set);
    iterator_2 = asGetNext(set);

    if(strcmp(iterator_1,iterator_2) >= 0)
    {
        printf("order test faild\n");
        asDestroy(set);
        return TEST_FAILD;
    }
    iterator_1 = iterator_2;
    iterator_2 = asGetNext(set);
    while (iterator_2 != NULL)
    {
        if(strcmp(iterator_1,iterator_2) >= 0)
        {
            printf("order test faild\n");
            asDestroy(set);
            return TEST_FAILD;
        }
        iterator_1 = iterator_2;
        iterator_2 = asGetNext(set);
    }

    asDestroy(set);
    return TEST_SUCCESS;
}

static TestAmountSetResult asChangeAmountTestNull()
{
    if(asChangeAmount(NULL, NULL , 10.3) != AS_NULL_ARGUMENT)
    {
        return TEST_FAILD;
    }
    if(asChangeAmount(NULL, "abc", 10) != AS_NULL_ARGUMENT)
    {
        return TEST_FAILD;
    }
    return TEST_SUCCESS;
}

TestAmountSetResult asChangeAmountTest()
{
    AmountSet set = asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }

    if(asChangeAmountTestNull() == TEST_FAILD)
    {
        asDestroy(set);
        return TEST_OUT_OF_MEMORY;
    }

    if(asChangeAmount(set, NULL, -10) != AS_NULL_ARGUMENT)
    {
        asDestroy(set);
        return TEST_FAILD;
    }

    asRegister(set, "abc");
    asRegister(set, "ggd");
    asRegister(set, "ASDG");

    double tester;
    asChangeAmount(set, "abc", 10);
    asGetAmount(set, "abc", &tester);
    if(tester != 10)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    asChangeAmount(set, "abc", 50);
    asGetAmount(set, "abc", &tester);
    if(tester != 60)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    
    asChangeAmount(set, "abc", -30);
    asGetAmount(set, "abc", &tester);
    if(tester != 30)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    if(asChangeAmount(set,"abc", -10000) != AS_INSUFFICIENT_AMOUNT)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    asDestroy(set);
    return TEST_SUCCESS;
}

static TestAmountSetResult asDeleteTestNull(AmountSet set)
{
    if(asDelete(set ,NULL) != AS_NULL_ARGUMENT)
    {
        return TEST_FAILD;
    }
    if(asDelete(NULL,"abc") != AS_NULL_ARGUMENT)
    {
        return TEST_FAILD;
    }
    if(asDelete(NULL,NULL) != AS_NULL_ARGUMENT)
    {
        return TEST_FAILD;
    }
    return TEST_SUCCESS;
}

TestAmountSetResult asDeleteTest()
{
    AmountSet set = asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }

    if(asDeleteTestNull(set) == TEST_FAILD)
    {
        asDestroy(set);
        return TEST_FAILD;
    }

    if(asRegisterWithAmount(set) == TEST_OUT_OF_MEMORY)
    {
        asDestroy(set);
        return TEST_OUT_OF_MEMORY;
    }
    if(asDelete(set, "tttttt") != AS_ITEM_DOES_NOT_EXIST)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    if(asDelete(set, "0000dfs2") != AS_SUCCESS)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    if(asContains(set,"0000dfs2") == true)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    asRegister(set,"0000dfs2");
    if(asDelete(set, "0000dfs2") != AS_SUCCESS)
    {
        asDestroy(set);
        return TEST_FAILD;
    }

    if(asContains(set,"0000dfs2") == true)
    {
        asDestroy(set);
        return TEST_FAILD;
    }

    if(asDelete(set, "!@RFu9o4") != AS_SUCCESS)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    if(asContains(set,"!@RFu9o4") == true)
    {
        asDestroy(set);
        return TEST_FAILD;
    }

    asDestroy(set);
    return TEST_SUCCESS;
}

TestAmountSetResult asClearTest()
{
    AmountSet set = asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }
    if(asClear(NULL) != AS_NULL_ARGUMENT)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    if(asClear(set)!= AS_SUCCESS)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    if(asRegisterWithAmount(set) == TEST_OUT_OF_MEMORY)
    {
        asDestroy(set);
        return TEST_OUT_OF_MEMORY;
    }
    if(asClear(set)!= AS_SUCCESS)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    if(asContains(set,"abcd1") == true)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
        if(asRegisterWithAmount(set) == TEST_OUT_OF_MEMORY)
    {
        asDestroy(set);
        return TEST_OUT_OF_MEMORY;
    }
    if(asClear(set)!= AS_SUCCESS)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    if(asContains(set,"abcd1") == true)
    {
        asDestroy(set);
        return TEST_FAILD;
    }
    asDestroy(set);
    return TEST_SUCCESS;
}

TestAmountSetResult asGetFirstTestNull()
{
    if(asGetFirst(NULL) == NULL)
    {
        return TEST_SUCCESS;
    }
    printf("null test of asGetFirst failed");
    return TEST_FAILD;
}

TestAmountSetResult asGetFirstTestEmptySet()
{
    AmountSet empty_set = asCreate();
    if(empty_set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }
    
    if(asGetFirst(empty_set) == NULL)
    {
        asDestroy(empty_set);
        return TEST_SUCCESS;
    }

    asDestroy(empty_set);
    printf("Empty set test of asGetFirst failed");
    return TEST_FAILD;
}

TestAmountSetResult asGetFirstTest()
{
    AmountSet set = asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }

    asRegister(set, "abcd");
    asRegister(set, "tuv");
    asRegister(set, "xyz");
    if ( (strcmp(asGetFirst(set), "abcd")) != 0  ){
        printf("basic test of asGetFirst failed");
        asDestroy(set);
        return TEST_FAILD;
    }

    asDelete(set, "abcd");
    if ( (strcmp(asGetFirst(set), "tuv")) != 0  ){
        printf("post delete test of asGetFirst failed");
        asDestroy(set);
        return TEST_FAILD;
    }

    asRegister(set, "efg");
    if ( (strcmp(asGetFirst(set), "efg")) != 0  ){
        printf("post register test of asGetFirst failed");
        asDestroy(set);
        return TEST_FAILD;
    }

    asDestroy(set);
    return TEST_SUCCESS;

}

TestAmountSetResult asGetNextTestNull()
{
    if(asGetNext(NULL) == NULL)
    {
        return TEST_SUCCESS;
    }
    printf("null test of asGetNext failed");
    return TEST_FAILD;
}


TestAmountSetResult asGetNextTest()
{
    AmountSet set = asCreate();
    if(set == NULL)
    {
        return TEST_OUT_OF_MEMORY;
    }

    asRegister(set, "abcd");
    asRegister(set, "tuv");
    asRegister(set, "xyz");
    asGetFirst(set);

    if ((strcmp(asGetNext(set), "tuv")) != 0)
    {
        printf("basic test of asGetNext failed");
        asDestroy(set);
        return TEST_FAILD;
    }

    asDelete(set, "tuv");
    asGetFirst(set);
    if ((strcmp(asGetNext(set), "xyz")) != 0)
    {
        printf("post delete test of asGetNext failed");
        asDestroy(set);
        return TEST_FAILD;
    }

    asDestroy(set);
    return TEST_SUCCESS;
}