#include <iostream>

// Header of the dynamic loading library
#include <dlfcn.h>

#include <stateful_writer_imports.h>

// If any error happens, get a description of the error
void _print_error(void *lib, const char *name) {
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load '" << name << "': " << dlsym_error << std::endl;
        dlclose(lib);
    }
}

// Loads a library and returns a pointer to the .so instance in memory
void *load_lib(const char *lib_name) {
    void *lib = dlopen(lib_name, RTLD_NOW);
    if(lib == nullptr) {
        _print_error(lib, lib_name);
    }
    return lib;
}

// Returns a pointer to an exported function from the library
void *load_function(void *lib_handle, const char *name) {
    void *p = dlsym(lib_handle, name);
    if(p == nullptr) {
        _print_error(p, name);
    }
    return p;
}

int main(int argc, char **argv) {

    if(argc <= 1) {
        std::cout << "Use: " << argv[0] << " <library_name>" << std::endl;
        return 1;
    }

    // Getting the library filename as 1st parameter
    const char *lib_filename = argv[1];
  
    void *lib = load_lib(lib_filename);
    if(lib == nullptr) { return 1; }

    new_writer_f *new_writer = (new_writer_f *)load_function(lib, "new_writer");
    if(lib == nullptr) { return 2; }

    delete_writer_f *delete_writer = (delete_writer_f *)load_function(lib, "delete_writer");
    if(lib == nullptr) { return 2; }

    stateful_increment_f *stateful_increment = (stateful_increment_f *)load_function(lib, "stateful_increment");
    if(lib == nullptr) { return 2; }

    stateful_write_f *stateful_write = (stateful_write_f *)load_function(lib, "stateful_write");
    if(lib == nullptr) { return 2; }

    void *writer = new_writer();

    stateful_write(writer, "Exterminate! Exterminate!");
    stateful_write(writer, "Delete! Delete!");

    std::cout << "How much: " << stateful_increment(writer, 100) << std::endl;

    // We cannot forget to destroy the object
    delete_writer(writer);

    dlclose(lib);
}
