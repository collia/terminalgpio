/* This file contains declaration and implementation ring buffer */

#define RING_BUFFER_DECL(name, size) \
    uint8_t name##Buffer[size]; \
    uint32_t name##BufPtrIn = 0;\
    uint32_t name##BufPtrOut = 0;

#define RING_BUFFER(name) name##Buffer
#define RING_BUFFER_IN_PTR(name) name##BufPtrIn
#define RING_BUFFER_OUT_PTR(name) name##BufPtrOut

#define RING_BUFFER_RESET(name) \
    name##BufPtrIn = 0;          \
    name##BufPtrOut = 0;


#define RING_BUFFER_INSERT(name, data, len) \
    {\
    uint8_t *__buffptr;\
    uint32_t __buffsize;\
    __buffptr = name##Buffer + name##BufPtrIn;             \
    if (name##BufPtrOut > name##BufPtrIn) {              \
        __buffsize = name##BufPtrOut - name##BufPtrIn - 1;     \
        if(len > __buffsize) {                               \
            memcpy(__buffptr, data, __buffsize);               \
            name##BufPtrIn +=__buffsize;                      \
        } else {                                           \
            memcpy(__buffptr, data, len);                    \
            name##BufPtrIn += len;                         \
        }                                                 \
    } else {                                              \
        if (name##BufPtrIn + len >= sizeof(name##Buffer)) {        \
            __buffsize =  sizeof(name##Buffer) - name##BufPtrIn ;    \
            memcpy(__buffptr, data, __buffsize);                        \
            __buffsize = name##BufPtrOut;                                \
            if(len -  __buffsize > __buffsize) {                        \
                memcpy(name##Buffer, data, __buffsize);                  \
                name##BufPtrIn =__buffsize;                              \
            } else {                                                    \
                memcpy(name##Buffer,                                     \
                       data + __buffsize,                               \
                       len - __buffsize);                               \
                name##BufPtrIn = len - __buffsize;                       \
            }                                                           \
        } else {                                                        \
            memcpy(__buffptr, data, len);                             \
            name##BufPtrIn += len;                                   \
        }                                                           \
    }                                                               \
    }


