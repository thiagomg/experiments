#include <iostream>

#include <dlfcn.h>
#include <writer_imports.h>

void _print_error(void *lib, const char *name) {
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load '" << name << "': " << dlsym_error << std::endl;
        dlclose(lib);
    }
}

void *load_lib(const char *lib_name) {
    void *lib = dlopen(lib_name, RTLD_NOW);
    if(lib == nullptr) {
        _print_error(lib, lib_name);
    }
    return lib;
}

void *load_function(void *lib_handle, const char *name) {
    void *p = dlsym(lib_handle, name);
    if(p == nullptr) {
        _print_error(p, name);
    }
    return p;
}


int main(int argc, char **argv) {

    void *lib = load_lib("/home/thiago/src/teste/lib/libstateful_writer.so");
    if(lib == nullptr) {
        return 1;
    }

    new_writer_f *new_writer = (new_writer_f *)load_function(lib, "new_writer");
    if(new_writer == nullptr) {
        return 1;
    }

    delete_writer_f *delete_writer = (delete_writer_f *)load_function(lib, "delete_writer");
    if (delete_writer == nullptr) {
        return 1;
    }

    stateful_increment_f *stateful_increment = (stateful_increment_f *)load_function(lib, "stateful_increment");
    if (stateful_increment == nullptr) {
        return 1;
    }

    stateful_write_f *stateful_write = (stateful_write_f *)load_function(lib, "stateful_write");
    if (stateful_write == nullptr) {
        return 1;
    }

    void *writer = new_writer();
    std::cout << "how: " << stateful_increment(writer, 1) << std::endl;
    stateful_write(writer, "Thiago");
    stateful_write(writer, "Massari");
    stateful_write(writer, "Guedes");

    delete_writer(writer);
}