//
// Created by Kevin Yu on 3/12/16.
//
#pragma once

#include <falton/setting.h>

class ftIntQueue {

private:
    uint32* handle;
    uint32 capacity;
    uint32 size;
    uint32 front;
    uint32 end;

public:

    void init();
    void cleanup();

    void push(uint32 handle);

    uint32 pop();

    uint32 getSize();

};

