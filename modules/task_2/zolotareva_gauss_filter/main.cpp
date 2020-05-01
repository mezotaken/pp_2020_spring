// Copyright 2020 lesya89
#include "./gauss_filter.h"
#ifndef OPENCV
#include "gtest/gtest.h"

TEST(sequential_gauss_filter, can_create_random_image) {
    ASSERT_NO_THROW(Image test = Image(10, 10, true));
}

TEST(sequential_gauss_filter, image_copying_is_correct) {
    Image test = Image(10, 10, true);
    Image test2(test);
    EXPECT_EQ((test == test2), true);
}

TEST(sequential_gauss_filter, filtering_is_correct) {
    unsigned char src[9] = { 16, 16, 16, 16, 32, 16, 16, 16, 16 };
    unsigned char dst[9] = { 0, 0, 0, 0, 20, 0, 0, 0, 0 };
    Image in = Image(3, 3, src);
    Image out = Image(3, 3, dst);
    EXPECT_EQ((in.GaussFilterOMP(1) == out), true);
}

TEST(sequential_gauss_filter, same_result_with_multithreading) {
    Image test = Image(16, 16, true);
    EXPECT_EQ((test.GaussFilterOMP(1) == test.GaussFilterOMP(4)), true);
}

TEST(sequential_gauss_filter, image_changes_after_filtering) {
    Image test = Image(16, 16, true);
    EXPECT_NE(test, test.GaussFilterOMP(4));
}

TEST(sequential_gauss_filter, can_filter_large_image_parallel) {
    Image test = Image(40, 40, true);
    ASSERT_NO_THROW(test.GaussFilterOMP(4));
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
    // ��� ������������ � opencv

    //cv::Mat src = cv::imread("C:\\test.jpg", 2);
    //cv::imshow("Source image", src);
    //Image test = Image(src.rows, src.cols, src.data);
    //Image res = test.GaussFilterOMP(4);
    //cv::Mat dst = cv::Mat(res.rows, res.cols, CV_8UC1, res.data);
    //cv::imshow("Result image", dst);
    //cv::waitKey(0);

    // ��� ������������ ���������
    int size = 5000;
    int threads = 4;
    Image test = Image(size, size, true);
    double t1 = omp_get_wtime();
    Image res = test.GaussFilterOMP(1);
    t1 = omp_get_wtime() - t1;
    std::cout << "Sequential time: " << t1 << "\n";
    double t2 = omp_get_wtime();
    res = test.GaussFilterOMP(threads);
    t2 = omp_get_wtime() - t2;
    std::cout << "Parallel time with " << threads << " threads: " << t2 << "\n";
    std::cout << "Speedup: " << t1 / t2 << "\n";
    return 0;
#endif
}
