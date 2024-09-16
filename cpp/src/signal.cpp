#include <csignal>
#include <fcntl.h> // For open(), O_RDONLY, O_CLOEXEC
#include <unistd.h> // For close() function

#include <base.h>

namespace projsignal {
    static void sighandle(int sig){
            signal(sig, SIG_DFL);
            int clifd = open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
            char buf[1024];
            ssize_t bufsize __attribute__((unused)) = read(clifd, buf, sizeof(buf));
            std::cout << "Fatal error, the program has stopped.\n";
            exit(127);
    }

    void register_signal(void){
            signal(SIGABRT, sighandle);
            signal(SIGBUS, sighandle);
            signal(SIGFPE, sighandle);
            signal(SIGILL, sighandle);
            signal(SIGQUIT, sighandle);
            signal(SIGSEGV, sighandle);
            signal(SIGSYS, sighandle);
            signal(SIGTRAP, sighandle);
            signal(SIGXCPU, sighandle);
            signal(SIGXFSZ, sighandle);
    }

}
