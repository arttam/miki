### abort vs exit vs terminat

#### exit
The following things happen when a program calls exit():

- Functions registered by the atexit function are executed
- All open streams are flushed and closed, files created by tmpfile are removed
- The program terminates with the specified exit code to the host

#### abort
The abort() function sends the SIGABRT signal to the current process, \
if it is not caught the program is terminated with no guarantee that open streams \
are flushed/closed or that temporary files created via tmpfile are removed, \
atexit registered functions are not called, and a non-zero exit status is returned to the host.

#### terminate
std::terminate is what is automatically called in a C++ program when there is an unhandled exception. \
This is essentially the C++ equivalent to abort, assuming that you are reporting all your exceptional errors by means of throwing exceptions. \
This calls a handler that is set by the std::set_terminate function, \
which by default simply calls abort.