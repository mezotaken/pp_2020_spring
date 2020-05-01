// Copyright 2020 lesya89
#include "./gauss_filter.h"
#ifndef OPENCV
#include "gtest/gtest.h"

TEST(tbb_gauss_filter, can_create_random_image) {
    ASSERT_NO_THROW(Image test = Image(10, 10, true));
}

TEST(tbb_gauss_filter, image_copying_is_correct) {
    Image test = Image(10, 10, true);
    Image test2(test);
    EXPECT_EQ((test == test2), true);
}

TEST(tbb_gauss_filter, filtering_is_correct) {
    unsigned char src[9] = { 16, 16, 16, 16, 32, 16, 16, 16, 16 };
    unsigned char dst[9] = { 0, 0, 0, 0, 20, 0, 0, 0, 0 };
    Image in = Image(3, 3, src);
    Image out = Image(3, 3, dst);
    EXPECT_EQ((in.GaussFilterTBB(1) == out), true);
}

TEST(tbb_gauss_filter, same_result_with_multithreading) {
    Image test = Image(16, 16, true);
    EXPECT_EQ((test.GaussFilterTBB(1) == test.GaussFilterTBB(4)), true);
}

TEST(tbb_gauss_filter, image_changes_after_filtering) {
    Image test = Image(16, 16, true);
    EXPECT_NE(test, test.GaussFilterTBB(4));
}

TEST(tbb_gauss_filter, can_filter_large_image_parallel) {
    Image test = Image(40, 40, true);
    ASSERT_NO_THROW(test.GaussFilterTBB(4));
}

#else
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#endif

int main(int argc, char **argv) {
#ifndef OPENCV
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#else
     // OpenCV demo

    // cv::Mat src = cv::imread("C:\\test.jpg", 2);
    // cv::imshow("Source image", src);
    // Image test = Image(src.rows, src.cols, src.data);
    // Image res = test.GaussFilterTBB(4);
    // cv::Mat dst = cv::Mat(res.rows, res.cols, CV_8UC1, res.data);
    // cv::imshow("Result image", dst);
    // cv::waitKey(0);

    // Speedup Demo
    int size = 5000;
    int threads = 4;
    Image test = Image(size, size, true);
    tbb::tick_count t = tbb::tick_count::now();
    Image res = test.GaussFilterTBB(1);
    double t1 = (tbb::tick_count::now() - t).seconds();
    std::cout << "Sequential time: " << t1 << "\n";
    t = tbb::tick_count::now();
    res = test.GaussFilterTBB(threads);
    double t2 = (tbb::tick_count::now() - t).seconds();
    std::cout << "Parallel time with " << threads << " threads: " << t2 << "\n";
    std::cout << "Speedup: " << t1/t2 << "\n";
    return 0;
#endif
}
