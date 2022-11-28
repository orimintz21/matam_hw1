#ifndef AMOUNT_SET_STR_TEST_H_
#define AMOUNT_SET_STR_TEST_H_

#include "amount_set_str.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



typedef enum TestAmountSetResult_t {
    TEST_SUCCESS = 0,
    TEST_FAILD,
    TEST_OUT_OF_MEMORY,
    TEST_NULL_ARGUMENT,
    TEST_ITEM_ALREADY_EXISTS,
    TEST_ITEM_DOES_NOT_EXIST,
    TEST_INSUFFICIENT_AMOUNT
} TestAmountSetResult;

#define RUN_TEST(test) \
do{ \
    printf("Running " #test "... "); \
    if(test() == TEST_SUCCESS) \
    { \
        printf("[OK]"); \
    } \
    else if(test() == TEST_OUT_OF_MEMORY) \
    { \
        printf("[out of memory]");\
    }\
    else if(test() == TEST_FAILD) \
    { \
        printf("[FAILD]"); \
    } \
    printf("\n");\
} while(0)

/**
 * @brief checks if the set was created properly
 * if we got the pointer for the set
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asCreatTest();

/**
 * @brief copy get NULL we expect to get NULL in return
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asCopyTestNull();

/**
 * @brief copy get an empty set
 * 
 * @return true 
 * @return false 
 */
TestAmountSetResult asCopyTestEmpty();

/**
 * @brief copy get a set, checks if the sets are the same
 * check if ther are elements with name and whithout amount
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asCopyTest();



/**
 * @brief asGetSize gets a Null pointer, should retern -1
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asGetSizeTestNull();


/**
 * @brief asGetSize gets an empty set, should retrun 0
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asGetSizeTestEmptySet();


/**
 * @brief asGetSize gets a full set, sould return the correct size
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asGetSizeTestFull();
/**
 * @brief asContains get a Null pointer for set should return false
 * asContains get a Null pointer for elemnt
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asContainsTestNullSetOrElement();



/**
 * @brief check the folloing sanerios:
 * set doesn't conteins the element
 * set conteins the element
 * element has been deleted
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asContainsTestFull();


/**
 * @brief  check the folloing sanerios:
 * set doesn't conteins the element
 * set conteins the element
 * set conteins the element whith 0
 * element has been deleted
 * e
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asGetAmountTestFull();

/**
 * @brief check the folloing sanerios:
 * null pointer
 * adds an element
 * adds the same element
 * adds 5 elements and checks the order
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asRegisterTest();

/**
 * @brief check the folloing sanerios:
 * null pointer
 * null pointer with 0
 * gets an element that isn't in the set
 * adds 0
 * adds posetive
 * adds twice to the same elemnt
 * subtruct (element remand posetive)
 * subtruct more then element amount
 * @return TestAmountSetResult 
 */
TestAmountSetResult asChangeAmountTest();

/**
 * @brief test the folloing sanerios:
 * null pointers
 * item doesn't exist
 * item exist whith 0
 * item exist
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asDeleteTest();

/**
 * @brief test the folloing sanerios:
 * null pointers
 * empty set
 * full set
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asClearTest();

/**
 * @brief asGetFirst gets a Null pointer, should return NULL
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asGetFirstTestNull();


/**
 * @brief asGetFirst gets an empty set, should retrun NULL
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asGetFirstTestEmptySet();


/**
 * @brief test the folloing sanerios:
 * full set
 * full set after deleting the first element
 * full set after registering a new element that should be the first
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asGetFirstTest();

/**
 * @brief asGetNext gets a Null pointer, should return NULL
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asGetNextTestNull();

/**
 * @brief test the folloing sanerios:
 * full set
 * full set after deleting an element
 * 
 * @return TestAmountSetResult 
 */
TestAmountSetResult asGetNextTest();

#endif /* AMOUNT_SET_STR_TEST_H_ */