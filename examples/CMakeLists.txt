if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
if (MSVC)
  add_compile_options(
    /W4
    /EHsc
    -D_AMD64_
  )
else()
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      # clang version 17.0.6 failed with this option
      # TODO: check clang version
      #add_compile_options(-stdlib=libc++)
    else()
      add_compile_options(
        -Wlogical-op
        -Wnoexcept
        -Wstrict-null-sentinel
        -Wuseless-cast
      )
    endif()
    add_compile_options(
      -Wall
      -Wextra
      -Wpedantic
      -Wcast-align
      -Wcast-qual
      -Wctor-dtor-privacy
      -Wconversion -Wno-sign-conversion
      -Wdisabled-optimization
      -Wdouble-promotion
      -Wformat=2
      -Winit-self
      -Wmissing-include-dirs
      -Wold-style-cast
      -Woverloaded-virtual
      -Wredundant-decls
      -Wshadow
      -Wstrict-aliasing=1
      -Wstrict-overflow=5
      -Wswitch-default
      -Wundef
    )
endif()

add_executable(ALL_examples)
target_link_libraries(ALL_examples ${PROJECT_NAME}::${PROJECT_NAME})
set_target_properties(ALL_examples PROPERTIES
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_compile_definitions(ALL_examples PRIVATE
    ALL_EXAMPLES_AS_SINGLE_PRJ
)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} source_files)
foreach(src IN LISTS source_files)
  get_filename_component(name_we ${src} NAME_WE)
  target_sources(ALL_examples PRIVATE ${src})
  set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-DTHIS_EXAMPLE_FNAME=${name_we}")
endforeach()
target_precompile_headers(ALL_examples PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
)
target_sources(ALL_examples PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/example-def.h
  ${CMAKE_CURRENT_SOURCE_DIR}/pch.h
  ${CMAKE_SOURCE_DIR}/include/clipp.h
  ${CMAKE_SOURCE_DIR}/include/clipp-narrow.h
  ${CMAKE_SOURCE_DIR}/include/clipp-wide.h
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${source_files})
set_target_properties(ALL_examples PROPERTIES FOLDER "Examples")
source_group("CLIPP" FILES
  ${CMAKE_SOURCE_DIR}/include/clipp.h
  ${CMAKE_SOURCE_DIR}/include/clipp-narrow.h
  ${CMAKE_SOURCE_DIR}/include/clipp-wide.h
)

#else()
#  foreach(src IN LISTS source_files)
#    get_filename_component(name_we ${src} NAME_WE)
#    add_executable(test-${name_we} ${src})
#    target_link_libraries(test-${name_we} ${PROJECT_NAME}::${PROJECT_NAME})
#    set_target_properties(test-${name_we} PROPERTIES
#      CXX_STANDARD_REQUIRED ON
#      CXX_EXTENSIONS OFF
#    )
#    add_test(NAME ${name_we} COMMAND $<TARGET_FILE:test-${name_we}>)
#    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src})
#    set_target_properties(test-${name_we} PROPERTIES FOLDER "Tests")
#  endforeach()
#endif()
