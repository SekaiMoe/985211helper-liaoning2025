#ifdef __linux__

#include <filesystem>
#include <csignal>
#include <fcntl.h> // For open(), O_RDONLY, O_CLOEXEC
#include <unistd.h> // For close() function
#ifdef __GLIBC__
#include <execinfo.h> // For backtrace
#else
#define backtrace(array, size) 0 // for musl
#define backtrace_symbols(array, size) nullptr
#endif
#include <ctime> // For timestamp

#include <base.h>

void logError(const std::string& func, const std::string& file, int line) {
    const std::string RED = "\033[31m";
    const std::string RESET = "\033[0m";
    std::cerr << RED << "In " << func << "() in " << file << " line " << line << ":" << RESET << std::endl;
}

#define LOG_ERROR() logError(__func__, __FILE__, __LINE__)

namespace projsignal {
    static void write_log(const char* msg) {
        // 打开日志文件
        std::ofstream logfile("./program_crash.log", std::ios::app);
        if (logfile.is_open()) {
            // 获取当前时间戳
            std::time_t t = std::time(nullptr);
            char timestamp[100];
            std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

            // 写入日志信息
            logfile << "[" << timestamp << "] " << msg << std::endl;

            // 关闭日志文件
            logfile.close();
        }
    }

    static void sighandle(int sig){
        signal(sig, SIG_DFL);  // 恢复默认信号处理
        int clifd = open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
        char buf[1024];
        ssize_t bufsize __attribute__((unused)) = read(clifd, buf, sizeof(buf));
        close(clifd);

        // 记录崩溃日志
        std::string errorMsg = "Fatal error (" + std::to_string(sig) + "), the program has been stopped.";
        LOG_ERROR();
        std::cerr << errorMsg << std::endl;
        write_log(errorMsg.c_str());
        std::cout << "Log file path: " << std::filesystem::current_path() << "./program_crash.log" << std::endl;

        // 获取调用栈
        #ifdef __GLIBC__
        void *array[10];
        int size = backtrace(array, 10);  // 将 size 的类型改为 int
        char **stackTrace = backtrace_symbols(array, size);

        if (stackTrace) {
            std::cout << "Stack trace:" << std::endl;
            write_log("Stack trace:");
            for (int i = 0; i < size; i++) {  // 使用 int 类型
                std::cout << stackTrace[i] << std::endl;
                write_log(stackTrace[i]);
            }
            free(stackTrace);
        }
        #else
        write_log("Stack trace not available.");
        #endif
        exit(127);
    }

    void register_signal(void){
        signal(SIGABRT, sighandle);
        signal(SIGBUS, sighandle);
        signal(SIGFPE, sighandle);
        signal(SIGILL, sighandle);
        signal(SIGQUIT, sighandle);
        signal(SIGSEGV, sighandle);  // 段错误处理
        signal(SIGSYS, sighandle);
        signal(SIGTRAP, sighandle);
        signal(SIGXCPU, sighandle);
        signal(SIGXFSZ, sighandle);
    }
}

#endif
