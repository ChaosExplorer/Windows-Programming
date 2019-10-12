#include <iostream>
#include "client/linux/handler/exception_handler.h"
#include "client/linux/handler/minidump_descriptor.h"

using namespace google_breakpad;
using std::cout;
using std::endl;

static bool dumpCallback(const google_breakpad::MinidumpDescriptor& des, void* context, bool succeed)
{
    return succeed;
}

#pragma pack(show)
#pragma pack(4)
struct peer
{
    peer(): index(0), name(NULL){};
    int index;
    char* name;
};

int main() {

    google_breakpad::MinidumpDescriptor descriptor("/home/chaos/projects/programming-lib/intern/SEGV/log");
    google_breakpad::ExceptionHandler eh(descriptor, NULL, dumpCallback, NULL, true, -1);

    peer* p = (peer*)calloc(2,sizeof(peer));

    p->index = 1; p->name = (char*)malloc(5);
    const char str[] = "fuck";
    cout << sizeof(str) << endl;
    //memcpy(p->name, str, sizeof(str));

    p->name = const_cast<char*>(str);
    cout << p->name << endl;

    cout << p << endl;
    cout << static_cast<const void*>(p->name) << endl;

    cout << sizeof(peer) << endl;

    peer t;
    cout << t.index << endl;
    cout << static_cast<const void*> (t.name) << endl;

    cout << p+1 << endl;
    cout << static_cast<const void*>((p+1)->name) << endl;
    cout << (p+1)->index <<endl;

    cout << p+2 << endl;
    cout << static_cast<const void*>((p+2)->name) << endl;
    cout << (p+2)->index <<endl;

    cout << p+3 << endl;
    cout << static_cast<const void*>((p+3)->name) << endl;
    cout << (p+3)->index <<endl;

    free(p+1);
    //free((p+1)->name);

    return 0;
}