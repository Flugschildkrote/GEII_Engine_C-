#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#define ENGINE_DEBUG 1
#define ENGINE_LOG_FULL_COLOR 0

#include <iostream>
#include <windows.h>

namespace GEII
{
    #ifdef _WIN32
       /* enum class Win32ConsoleColor
        {
            DARK_RED =    FOREGROUND_RED ,
            DARK_BLUE =   FOREGROUND_BLUE ,
            DARK_GREEN =  FOREGROUND_GREEN ,
            DARK_YELLOW = OREGROUND_RED | FOREGROUND_GREEN ,
            DARK_WHITE =  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE ,
            RED =    FOREGROUND_RED | FOREGROUND_INTENSITY,
            BLUE =   FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            GREEN =  FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            YELLOW = OREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            WHITE =  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
        };*/

        #define WIN_DARK_RED    FOREGROUND_RED
        #define WIN_DARK_BLUE   FOREGROUND_BLUE
        #define WIN_DARK_GREEN  FOREGROUND_GREEN
        #define WIN_DARK_YELLOW FOREGROUND_RED  | FOREGROUND_GREEN
        #define WIN_DARK_WHITE  FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE
        #define WIN_DARK_CYAN   FOREGROUND_GREEN | FOREGROUND_BLUE
        #define WIN_RED         FOREGROUND_RED   | FOREGROUND_INTENSITY
        #define WIN_BLUE        FOREGROUND_BLUE  | FOREGROUND_INTENSITY
        #define WIN_GREEN       FOREGROUND_GREEN | FOREGROUND_INTENSITY
        #define WIN_YELLOW      FOREGROUND_RED  | FOREGROUND_GREEN | FOREGROUND_INTENSITY
        #define WIN_WHITE       FOREGROUND_RED  | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY

        static void WIN32_SET_CONSOLE_COLOR(WORD color){
            HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hStdout, color);
        }
    #else
       // #define WIN32_SET_CONSOLE_COLOR(color)
    #endif // _WIN32*/

    class Logger
    {
    public :
        Logger(const std::string &str)
            : mName(str)
        {
        }

        virtual ~Logger(void)
        {

        }

        template<typename T>
        void Info(const T& info) const
        {
            beginLog(WIN_DARK_CYAN);
            sendLog(info);
            endLog();
        }

        template<typename T, typename ...Args>
        void Info(const T& info, const Args& ...args) const
        {
            beginLog(WIN_DARK_CYAN);
            sendLog(info);
            sendLog(args...);
            endLog();
        }

        template<typename T>
        void Success(const T& success) const
        {
            beginLog(WIN_GREEN);
            sendLog(success);
            endLog();
        }

        template<typename T, typename ...Args>
        void Success(const T& succes, const Args& ...args) const
        {
            beginLog(WIN_GREEN);
            sendLog(succes);
            sendLog(args...);
            endLog();
        }

        template<typename T>
        void Error(const T& error)
        {
            beginLog(WIN_RED);
            sendLog(error);
            endLog();
        }

        template<typename T, typename... Args>
        void Error(const T& error, const Args&... args)
        {
            beginLog(WIN_RED);
            sendLog(error);
            sendLog(args...);
            endLog();
        }

        template<typename T>
        void Warning(const T& warning)
        {
            beginLog(WIN_DARK_YELLOW);
            sendLog(warning);
            endLog();
        }

        template<typename T, typename... Args>
        void Warning(const T& warning, const Args&... args)
        {
            beginLog(WIN_DARK_YELLOW);
            sendLog(warning);
            sendLog(args...);
            endLog();
        }

    protected :
        void beginLog(WORD color) const
        {
            WIN32_SET_CONSOLE_COLOR(color);
            std::cerr << "[" << mName << "] : ";
            #if ENGINE_LOG_FULL_COLOR == 0
                WIN32_SET_CONSOLE_COLOR(WIN_WHITE);
            #endif // ENGINE_LOG_FULL_COLOR

        }

        void endLog(void) const{
            std::cerr << "\n";
            WIN32_SET_CONSOLE_COLOR(WIN_WHITE);
        }

        template<typename T>
        void sendLog(const T& log) const
        {
            std::cout << log;
        }

        template<typename T, typename... Args>
        void sendLog(const T& log, const Args&... args) const
        {
            sendLog(log);
            sendLog(args...);
        }

        std::string mName;
    };

    extern Logger EngineConsoleLogger;
}

#if ENGINE_DEBUG == 1
    #define ENGINE_LOG_INFO(...) GEII::EngineConsoleLogger.Info(__VA_ARGS__)
    #define ENGINE_LOG_ERROR(...) GEII::EngineConsoleLogger.Error(__VA_ARGS__)
    #define ENGINE_LOG_WARNING(...) GEII::EngineConsoleLogger.Warning(__VA_ARGS__)
    #define ENGINE_LOG_SUCCESS(...) GEII::EngineConsoleLogger.Success(__VA_ARGS__)
#else
    #define ENGINE_LOG_INFO(...)
    #define ENGINE_LOG_ERROR(...)
    #define ENGINE_LOG_WARNING(...)
    #define ENGINE_LOG_SUCCESS(...)
#endif // ENGINE_DEBUG

#endif // LOG_H_INCLUDED
