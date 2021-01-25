add_executable(async_demo 
    main.cpp
    generator.cpp    
)
target_link_libraries(async_demo  
async
Threads::Threads
)

set_default_opts(async_demo)