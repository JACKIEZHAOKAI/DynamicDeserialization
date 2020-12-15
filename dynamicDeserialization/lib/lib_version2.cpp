#include "../reader/gross.h"
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define DATASET2_EXTENSION(StructName, ...) \
struct StructName { \
    __VA_ARGS__ \
} \

#define DATASET2_EXTENSION_Create(StructName) \
extern "C" void * Create(const std::vector<uint8_t> &payload) { \
    StructName *instance = new StructName(); \
    std::string string_data(payload.begin(), payload.end()); \
    std::istringstream data (string_data); \
    boost::property_tree::ptree root; \
    boost::property_tree::read_json(data, root); \
    try { \
         instance->foo = root.get<int>("foo"); \
    } \
    catch(const boost::property_tree::ptree_error &e) { \
        std::cerr << "Error in loading foo (missing key or wrong type)" << std::endl; \
        std::cerr << e.what() << std::endl; \
    } \
    try { \
         instance->bar = root.get<std::string>("bar"); \
    } \
    catch (const boost::property_tree::ptree_error &e) { \
        std::cerr << "Error in loading bar (missing key or wrong type)" << std::endl; \
        std::cerr << e.what() << std::endl; \
    } \
    try { \
         instance->odd = root.get<std::string>("odd"); \
    } \
    catch (const boost::property_tree::ptree_error &e) { \
        std::cerr << "Error in loading odd (missing key or wrong type)" << std::endl; \
        std::cerr << e.what() << std::endl; \
    } \
    return instance; \
} \

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

DATASET2_EXTENSION(
    WriterStruct,
    int foo;
    std::string bar;
    std::string odd; 
);
DATASET2_EXTENSION_Create(WriterStruct)
DATASET2_EXTENSION_GetField(WriterStruct) 
DATASET2_EXTENSION_Delete(WriterStruct)
