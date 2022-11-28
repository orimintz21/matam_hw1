#include "amount_set_str_tests.h"
#include "amount_set_str.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    RUN_TEST(asCreatTest);
    RUN_TEST(asCopyTestNull);
    RUN_TEST(asCopyTestEmpty);
    RUN_TEST(asCopyTest);
    RUN_TEST(asGetSizeTestNull);
    RUN_TEST(asGetSizeTestEmptySet);
    RUN_TEST(asGetSizeTestFull);
    RUN_TEST(asContainsTestNullSetOrElement);
    RUN_TEST(asContainsTestFull);
    RUN_TEST(asGetAmountTestFull);
    RUN_TEST(asRegisterTest);
    RUN_TEST(asChangeAmountTest);
    RUN_TEST(asDeleteTest);
    RUN_TEST(asClearTest);
    RUN_TEST(asGetFirstTestNull);
    RUN_TEST(asGetFirstTestEmptySet);
    RUN_TEST(asGetFirstTest);
    RUN_TEST(asGetNextTestNull);
    RUN_TEST(asGetNextTest);
    return 0;
}