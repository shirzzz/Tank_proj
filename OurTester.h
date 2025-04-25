

#ifndef OURTESTER_H
#define OURTESTER_H
#include <wchar.h>


class OurTester {
public:
    FILE buildRandomBoardFile();
    bool testLoadingBoardFromFile();
    bool testLoadingBoardFromFileWithErrors();
};



#endif //OURTESTER_H
