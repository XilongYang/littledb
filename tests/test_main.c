#include "unity.h"
#include "littledb.h"

void setUp(void) {}

void tearDown(void) {}

void version_test(void)
{
    TEST_ASSERT_EQUAL_STRING(get_version(), "0.0.1");
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(version_test);
    return UNITY_END();
}
