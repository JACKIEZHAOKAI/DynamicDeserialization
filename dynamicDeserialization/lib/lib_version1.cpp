#include <stdio.h>
#include <string>

#define DATASET2_EXTENSION(StructName, ...) \
struct StructName { \
    __VA_ARGS__ \
} \

// Provide a method to deserialize data into the C++ struct in message_definition 
//   `void * deserialize(const vector<Bytes> &data)`

// TODO: change to (payload is a serialized object of the WriterStruct):
// extern "C" void * Create(Bytes payload){ 
//     WriterStruct * ws = deserialize<WriterStruct>(payload);
//     return ws;
// }

#define DATASET2_EXTENSION_Create(StructName) \
extern "C"  void * Create() { \
    StructName *instance = new StructName(); \
    instance->foo = 100; \
    instance->odd = "lib1 odd"; \
    instance->bar = "lib1 bar"; \
    return instance; \
} \

// Provide a C-style object method through an ABI that gets fields from the C++ struct
//    `void * GetField(void * instance, string field)`
// Return address of the field in runtime memory 

// TODO: Find a way to traverse the WriterStruct to find the field to return 
// extern "C" void * GetField(void * instance, const char * field) {   //address of payload, field of the WriterStruct
//     WriterStruct * ws = (WriterStruct *)instance;
//     return &(ws->field);
// }
#define DATASET2_EXTENSION_GetField(StructName) \
extern "C" void * GetField(void * instance, std::string field) { \
    void * fieldPtr; \
    if (field.compare("foo") == 0) { \
        fieldPtr = &(((StructName *)instance)->foo); \
    } \
    else if (field.compare("bar") == 0) { \
        fieldPtr = &(((StructName *)instance)->bar); \
    } \
    else if (field.compare("odd") == 0) { \
        fieldPtr = &(((StructName *)instance)->odd); \
    } \
    else { \
        fieldPtr = NULL; \
    } \
    return fieldPtr;  \
} \

#define DATASET2_EXTENSION_Delete(StructName) \
extern "C" void Delete(void * instance) { \
    delete (StructName*) instance; \
} \

//defining the Marco functions here
DATASET2_EXTENSION(
    WriterStruct,
    int foo;
    std::string bar;
    std::string odd; 
);

DATASET2_EXTENSION_Create(WriterStruct)

DATASET2_EXTENSION_GetField(WriterStruct) 

DATASET2_EXTENSION_Delete(WriterStruct)