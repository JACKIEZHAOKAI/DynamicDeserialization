#include <stdio.h>
#include <string>

// message_definition is a C++ struct
struct WriterStruct {
  int foo;
  std::string bar;
  std::string odd;
};

extern "C" void * Create() {
    WriterStruct * ws = new WriterStruct();
    ws->foo = 100;
    ws->bar = "lib1 bar";
    ws->odd = "lib1 odd";
    return ws; 
}

extern "C" void * GetField(void * instance, std::string field) {   //address of payload, field of the WriterStruct
    void * fieldPtr;
    if (field.compare("foo") == 0) {
        fieldPtr = &(((WriterStruct *)instance)->foo);
    }
    else if (field.compare("bar") == 0) {
        fieldPtr = &(((WriterStruct *)instance)->bar);
    }
    else if (field.compare("odd") == 0) {
        fieldPtr = &(((WriterStruct *)instance)->odd);
    }
    else {
        fieldPtr = NULL;
    }
    return fieldPtr; 
}

extern "C" void Delete(void * instance) {
    delete (WriterStruct * )instance;
}