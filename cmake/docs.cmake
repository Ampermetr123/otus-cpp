# generate docs options
option(BUILD_DOCUMENTATION "Build documentation" ON)
# Documentation setup
if(BUILD_DOCUMENTATION)
    find_package(Doxygen
                REQUIRED dot
                OPTIONAL_COMPONENTS mscgen dia)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/docs")
        #install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${CMAKE_SOURCE_DIR}/docs)
        #set(DOXYGEN_PROJECT_LOGO "${CMAKE_SOURCE_DIR}/logo.png")
        set(DOXYGEN_GENERATE_TREEVIEW YES)
        set(DOXYGEN_HIDE_UNDOC_RELATIONS NO)
        set(DOXYGEN_HAVE_DOT YES)
        set(DOXYGEN_DOT_NUM_THREADS 4)
        set(DOXYGEN_UML_LOOK YES)
        set(DOXYGEN_UML_LIMIT_NUM_FIELDS 50)
        set(DOXYGEN_TEMPLATE_RELATIONS YES)
        set(DOXYGEN_DOT_IMAGE_FORMAT svg)
        set(DOXYGEN_INTERACTIVE_SVG YES)
        set(DOXYGEN_DOT_GRAPH_MAX_NODES 100)
        set(DOXYGEN_DOT_TRANSPARENT YES)
        set(DOXYGEN_DOT_MULTI_TARGETS YES)
        set(USE_MDFILE_AS_MAINPAGE YES)
        doxygen_add_docs(doc
          ${CMAKE_CURRNET_SOURCE_DIR}
          COMMENT "Generating doxygen documentation for project"
        )
    else()
    message(WARNING "doxygen and graphviz need to be installed to generate the doxygen documentation") 
    endif(DOXYGEN_FOUND)
endif(BUILD_DOCUMENTATION)