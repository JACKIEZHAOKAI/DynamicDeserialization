// POC #4 Generate the dynamic plugin(shared object) at runtime from 
// a source struct in a JSON file, and load it using details from POC #1.
#include <iostream>
#include <string>
#include <fstream>  // Writing the dynamic plugin lib.cpp file 
#include <boost/property_tree/ptree.hpp>    // loading json file
#include <boost/property_tree/json_parser.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>

#define PATH_SIZE 128
#define OFFSET 5
#define START_WORD "/data"
#define END_WORD "/meta.json"
#define PREFIX "../lib/lib"
#define COMPILE_DLL "g++ -fPIC -shared "
#define STARTINGBRACE "{"
#define ENDINGBRACE "}"

using namespace std;

// read a json file using C++ boost lib, return the WriterStruct definition
string read_meta_file (string file_path) {
  boost::property_tree::ptree root;
  cout << "\nReading meta.json file...\n";
  boost::property_tree::read_json(file_path, root);  // Load the json file in this ptree
  string definition = root.get<string>("definition");  //read and save the roll
  return definition;
}

// write the dynamic plugin and return its path
void write_dll_source_file(string dll_sourcecode_path, string writerStruct_fields) {  
  cout << "\nWriting dynamic plugin source code...\n";

  ofstream outFile(dll_sourcecode_path.c_str());
  const string text = "#include \"../reader/gross.h\"\n"
                      "#include <stdio.h>\n"
                      "#include <stdint.h>\n"
                      "#include <sstream>\n"
                      "#include <iostream>\n"
                      "#include <string>\n"
                      "#include <vector>\n"
                      "#include <boost/property_tree/ptree.hpp>\n"
                      "#include <boost/property_tree/json_parser.hpp>\n"
                      "\n"
                      "using namespace std;\n"
                      "\n"
                      "// for-each, two arg macro\n"
                      "#define FOREACHPAIR(F, ...) __TO_FOREACHPAIRN(COUNT_ARGS(__VA_ARGS__))(F, __VA_ARGS__)\n"
                      "#define __TO_FOREACHPAIRN(N) __CONCAT_FOREACHPAIR_N(N)\n"
                      "#define __CONCAT_FOREACHPAIR_N(N) FOREACHPAIR ## N\n"
                      "\n"
                      "// Definition of Marco STRUCT\n"
                      "#define STRUCT(S, ...) \\\n"
                      "struct S { \\\n"
                      "  STRUCT_FIELDS(__VA_ARGS__) \\\n"
                      "}\n"
                      "\n"
                      "// Definition of Marco FUNC\n"
                      "#define FUNC(S, ...) \\\n"
                      "  STRUCT_CREATE(S, __VA_ARGS__) \\\n"
                      "  STRUCT_GET_FIELD(__VA_ARGS__) \\\n"
                      "  STRUCT_DELETE(S) \\\n"
                      "\n"
                      "// Marco function of STRUCT_FIELDS\n"
                      "#define STRUCT_FIELDS_ONE(A, B) A B;\n"
                      "#define STRUCT_FIELDS(...) FOREACHPAIR(STRUCT_FIELDS_ONE, __VA_ARGS__)\n"
                      "\n"
                      "// Marco function of Create()\n"
                      "#define STRUCT_CREATE_ASSIGN_VALUE_ONE(A, B) \\\n"
                      "try { \\\n"
                      "    std::cout << root.get<A>(#B) << std::endl; \\\n"
                      "    instance->B = root.get<A>(#B); \\\n"
                      "} \\\n"
                      "catch(const boost::property_tree::ptree_error &e) { \\\n"
                      "    std::cerr << \"Error in loading \" << #B << \"(missing key or wrong type)\" << std::endl; \\\n"
                      "    std::cerr << e.what() << std::endl; \\\n"
                      "} \\\n"
                      "\n"
                      "#define STRUCT_CREATE_ASSIGN_VALUE(...) \\\n"
                      "FOREACHPAIR(STRUCT_CREATE_ASSIGN_VALUE_ONE, __VA_ARGS__)\n"
                      "\n"
                      "#define STRUCT_CREATE(StructName, ...) \\\n"
                      "extern \"C\" void * Create(const std::vector<uint8_t> &payload) { \\\n"
                      "    StructName *instance = new StructName(); \\\n"
                      "    std::string string_data(payload.begin(), payload.end()); \\\n"
                      "    std::cout << string_data << std::endl; \\\n"
                      "    std::istringstream data (string_data); \\\n"
                      "    boost::property_tree::ptree root; \\\n"
                      "    boost::property_tree::read_json(data, root); \\\n"
                      "    STRUCT_CREATE_ASSIGN_VALUE(__VA_ARGS__); \\\n"
                      "    return instance; \\\n"
                      "} \\\n"
                      "\n"
                      "// Marco function of GetField()\n"
                      "#define STRUCT_GET_FIELD_CHECK_FOR_ONE(A, B) \\\n"
                      "if (field.compare(#B) == 0) { \\\n"
                      "    fieldPtr = &(((WriterStruct *)instance)->B); \\\n"
                      "} \\\n"
                      "\n"
                      "#define STRUCT_GET_FIELD_CHECK(...) \\\n"
                      "FOREACHPAIR(STRUCT_GET_FIELD_CHECK_FOR_ONE, __VA_ARGS__)\n"
                      "\n"
                      "#define STRUCT_GET_FIELD(...) \\\n"
                      "extern \"C\" void * GetField(void * instance, std::string field) { \\\n"
                      "    void * fieldPtr = NULL; \\\n"
                      "    STRUCT_GET_FIELD_CHECK(__VA_ARGS__); \\\n"
                      "    return fieldPtr;  \\\n"
                      "} \\\n"
                      "\n"
                      "// Marco function of Delete()\n"
                      "#define STRUCT_DELETE(StructName) \\\n"
                      "extern \"C\" void Delete(void * instance) { \\\n"
                      "    delete (StructName*) instance; \\\n"
                      "} \\\n"
                      "\n"
                      "// ############ user input of WriterStruct ############\n"
                      "STRUCT(\n"
                      "  WriterStruct,\n" +
                        writerStruct_fields +
                      ");\n"
                      "// Max taking 8 fields\n"
                      "// char, field9 <-- error (even with comma after field8)\n"
                      "FUNC(\n"
                      "  WriterStruct,\n" +
                        writerStruct_fields +
                      ");";
                        
  // Write to the dll source code file
  outFile << text;

  // Close the file
  outFile.close();
}

// return the unique dll path
string generate_file_path(string file_path, string postfix) {
  int first = file_path.find(START_WORD);
  int last = file_path.find(END_WORD);
  string file_number = file_path.substr(first+OFFSET, last-first-OFFSET);
  return PREFIX + file_number + postfix;
}

void compile_dll(string dll_target_path, string dll_path) {
    cout << "\nCompiling the dynamic plugin source code...\n";
    FILE * fp = NULL;
    // The Linux command to compile the lib.cpp file into lib.so file
    string linux_command = COMPILE_DLL + dll_target_path + " -o " + dll_path;
    fp = popen(linux_command.c_str(),"w");

    if(fp){
        char pInfo[PATH_SIZE];
        memset(pInfo, 0, sizeof(pInfo));
        if(fgets(pInfo, sizeof(pInfo), fp)){
                printf("%s",pInfo);
                pclose(fp);
        }else
            pclose(fp);
    }
}

string extract_fields(string message_definition) {
  // extract the WriterStruct fields between the two braces, assuming only two brances
  unsigned first = message_definition.find(STARTINGBRACE);
  unsigned last = message_definition.find(ENDINGBRACE);
  string fields = message_definition.substr (first+1,last-first-1);  
  //eliminate ";"
  string result;
  remove_copy(fields.begin(), fields.end(), back_inserter(result), ';');
  //replace " " with ","  assuming there is space after each break 
  replace(result.begin(), result.end(), ' ', ',');
  return result;
}

// Return a string that is a path to the shared object
string make_plugin(string meta_file_path) {
  // 1. load JSON file (assuming the JSON file is valid), obtain WriterStruct 
  string message_definition = read_meta_file(meta_file_path);
  cout << "message_definition:\n" << message_definition << endl;

  // 2. extract WriterStruct fields and substitute into lib#.cpp source file
  string writerStruct_fields = extract_fields(message_definition);
  cout << "writerStruct_fields:\n" << writerStruct_fields << endl;

  // 3. Generate source code for a plugin (ex. write lib#.cpp)
  string dll_target_path = generate_file_path(meta_file_path, ".cpp");
  write_dll_source_file(dll_target_path, writerStruct_fields);
  cout << "dll_source_code_path: " << dll_target_path << endl;

  // 4. Compile the source code to a shared object (ex. lib#.cpp -> lib#.so)
  string dll_path = generate_file_path(meta_file_path, ".so");
  compile_dll(dll_target_path, dll_path); 

  return dll_path;
}