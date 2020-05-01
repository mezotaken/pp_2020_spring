// Copyright 2020 lesya89

#ifndef MODULES_TASK_3_ZOLOTAREVA_GAUSS_FILTER_GAUSS_FILTER_H_
#define MODULES_TASK_3_ZOLOTAREVA_GAUSS_FILTER_GAUSS_FILTER_H_

#include <tbb/tbb.h>
#include <random>
#include <iostream>
#include <ctime>
#include <cstring>

class Image {
 public:
    unsigned char *data;
    int rows;
    int cols;
    Image() {}
    Image(int r, int c, bool random);
    Image(int r, int c, unsigned char *d);
    Image(const Image& img);
    Image& operator=(const Image& right);
    bool operator== (const Image& img) const;
    bool operator!= (const Image& img) const { return !(*this == img); }
    Image GaussFilterTBB(int numthreads);
    ~Image();
};

class tbb_gauss {
 private:
    unsigned char *source;
    unsigned char *result;
    int rows;
    int cols;
 public:
    tbb_gauss(unsigned char *src, unsigned char *res, int r, int c);
    void operator() (const tbb::blocked_range<int> &range) const;
};

#endif  // MODULES_TASK_3_ZOLOTAREVA_GAUSS_FILTER_GAUSS_FILTER_H_
