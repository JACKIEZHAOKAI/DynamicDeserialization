#include "../reader/gross.h"
#include <stdio.h>
#include <stdint.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;

// for-each, two arg macro
#define FOREACHPAIR(F, ...) __TO_FOREACHPAIRN(COUNT_ARGS(__VA_ARGS__))(F, __VA_ARGS__)
#define __TO_FOREACHPAIRN(N) __CONCAT_FOREACHPAIR_N(N)
#define __CONCAT_FOREACHPAIR_N(N) FOREACHPAIR ## N

// Definition of Marco STRUCT
#define STRUCT(S, ...) \
struct S { \
  STRUCT_FIELDS(__VA_ARGS__) \
}

// Definition of Marco FUNC
#define FUNC(S, ...) \
  STRUCT_CREATE(S, __VA_ARGS__) \
  STRUCT_GET_FIELD(__VA_ARGS__) \
  STRUCT_DELETE(S) \

// Marco function of STRUCT_FIELDS
#define STRUCT_FIELDS_ONE(A, B) A B;
#define STRUCT_FIELDS(...) FOREACHPAIR(STRUCT_FIELDS_ONE, __VA_ARGS__)

// Marco function of Create()
#define STRUCT_CREATE_ASSIGN_VALUE_ONE(A, B) \
try { \
    std::cout << root.get<A>(#B) << std::endl; \
    instance->B = root.get<A>(#B); \
} \
catch(const boost::property_tree::ptree_error &e) { \
    std::cerr << "Error in loading " << #B << "(missing key or wrong type)" << std::endl; \
    std::cerr << e.what() << std::endl; \
} \

#define STRUCT_CREATE_ASSIGN_VALUE(...) \
FOREACHPAIR(STRUCT_CREATE_ASSIGN_VALUE_ONE, __VA_ARGS__)

#define STRUCT_CREATE(StructName, ...) \
extern "C" void * Create(const std::vector<uint8_t> &payload) { \
    StructName *instance = new StructName(); \
    std::string string_data(payload.begin(), payload.end()); \
    std::cout << string_data << std::endl; \
    std::istringstream data (string_data); \
    boost::property_tree::ptree root; \
    boost::property_tree::read_json(data, root); \
    STRUCT_CREATE_ASSIGN_VALUE(__VA_ARGS__); \
    return instance; \
} \

// Marco function of GetField()
#define STRUCT_GET_FIELD_CHECK_FOR_ONE(A, B) \
if (field.compare(#B) == 0) { \
    fieldPtr = &(((WriterStruct *)instance)->B); \
} \

#define STRUCT_GET_FIELD_CHECK(...) \
FOREACHPAIR(STRUCT_GET_FIELD_CHECK_FOR_ONE, __VA_ARGS__)

#define STRUCT_GET_FIELD(...) \
extern "C" void * GetField(void * instance, std::string field) { \
    void * fieldPtr = NULL; \
    STRUCT_GET_FIELD_CHECK(__VA_ARGS__); \
    return fieldPtr;  \
} \

// Marco function of Delete()
#define STRUCT_DELETE(StructName) \
extern "C" void Delete(void * instance) { \
    delete (StructName*) instance; \
} \

// ############ user input of WriterStruct ############
STRUCT(
  WriterStruct,
  int, foo,
  std::string, bar,
  std::string, odd
);
// Max taking 8 fields
// char, field9 <-- error (even with comma after field8)

FUNC(
  WriterStruct,
  int, foo,
  std::string, bar,
  std::string, odd
);
//######################################################