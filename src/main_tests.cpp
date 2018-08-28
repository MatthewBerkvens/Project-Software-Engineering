#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <gtest/gtest.h>

#include "utils/Utils.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}