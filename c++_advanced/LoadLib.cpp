//
// Created by chaos on 5/29/19.
//
#include <iostream>
#include <dlfcn.h>
#include <zlib.h>

using namespace std;

int main(int argc, char**argv) {
    const char *LIB_PATH = "/usr/local/lib/libz.so";

    // load .so lib
    void *handle = dlopen(LIB_PATH, RTLD_NOW);
    if (!handle) {
        cerr << "Open library " << LIB_PATH << "error : " << dlerror() << endl;
        return -1;
    }
    else
    {
        cout << "Open OK." << endl;
    }

    // find specified function
    gzFile (*fn_gzopen)(const char *path, const char *mode);

    fn_gzopen = (gzFile(*)(const char *path, const char *mode)) dlsym(handle, "gzopen");
    char *err = NULL;
    if ((err = dlerror()) != NULL)
    {
        cerr << "Symbol gzopen not found: " << err << endl;
        dlclose(handle);
    }
    else
    {
        cout << "gzopen found at : " << static_cast<void*>(&fn_gzopen) << endl;
    }

    fn_gzopen("path.gz", "rb");

    dlclose(handle);
    typedef int (*PFI)(int, const char *);
    return 0;
}