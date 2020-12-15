#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>  // provide API for opening a DLL library, looking up symbols, handling errors, and closing the library.
#include <string>

// zhaokai.xu@sd-lub-293c0d:~/Desktop/testDynamic$ cd lib/
// zhaokai.xu@sd-lub-293c0d:~/Desktop/testDynamic/lib$ gcc -fPIC -shared lib1.c -o lib1.so
// zhaokai.xu@sd-lub-293c0d:~/Desktop/testDynamic/lib$ gcc -fPIC -shared lib2.c -o lib2.so
// zhaokai.xu@sd-lub-293c0d:~/Desktop/testDynamic/lib$ cd ..
// zhaokai.xu@sd-lub-293c0d:~/Desktop/testDynamic$ gcc -o main main.c -ldl
// zhaokai.xu@sd-lub-293c0d:~/Desktop/testDynamic$ ./main ./lib/lib1.so
// GetNumber result : 1 
// zhaokai.xu@sd-lub-293c0d:~/Desktop/testDynamic$ ./main ./lib/lib2.so
// GetNumber result : 2 

int main(int argc, char **argv) {
    void *handle;
    char *error;
    int (*getNumber)();            
    
    // open the DLL with path as argv[1]
    handle = dlopen (argv[1], RTLD_LAZY);
    if (!handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }
    // load the func pointer
    // (actual type) dlsys(handle, symbol being searched for)
    getNumber = dlsym(handle, "GetNumber");
    
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        exit(1);
    }

    // the user should know what type to cast to
    string ret = getNumber();  // call the DLL function

    printf("GetNumber result : %f \n", ret);
 
    dlclose(handle);
}