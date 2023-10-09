#pragma once

#ifdef TEST
#define VIRTUAL virtual
#define VIRTUAL_DESTRUCTOR(type)                                               \
    virtual ~type() {}
#else
#define VIRTUAL
#define VIRTUAL_DESTRUCTOR(type)
#endif
