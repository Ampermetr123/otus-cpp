/**
 * @file async.h
 * @author Sergey Simonov (sb.simonov@gmail.com)
 * @brief Home work 8 - asyn library.  
 *        Separates incoming commands to bulks and outputs it to console and files.
 *        See datailed information in homework8.pdf.
 * @note  Supports multi thread, but each handle must be processed in its own thread.
 */
#pragma once

#include <cstddef>

#ifdef _WIN32
#  ifdef ASYNC_API_EXPORTS
#    define ASYNC_API __declspec(dllexport)
#  else
#    define ASYNC_API __declspec(dllimport)
#  endif
#else
#  define ASYNC_API
#endif

namespace async{

    using handle_t = void*;

    /**
     * @brief Open context for processing commands
     * @param bulk size of bulk
     * @return handle 
     */
    ASYNC_API handle_t connect(std::size_t bulk);

    /**
     * @brief Process command asynchronously. 
     * After bulk is ready do the next: 
     * - outputs bulks to console; 
     * - creates file with unique name and writes bulk to it.
     * @param handle handle, see async::connect
     * @param data command string
     * @param size length of command string
     * @note Each handle must be processed in it's own thread
     */
    ASYNC_API void receive(handle_t handle, const char* data, std::size_t size);

    /**
     * @brief Close handle.
     * Function bloks calling thread until all bulks on handle are processed.
     */   
    ASYNC_API void disconnect(handle_t handle);
}