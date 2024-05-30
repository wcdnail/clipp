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
    add_compile_options(-stdlib=libc++)
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

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} source_files)
if (BUILD_TESTING_SINGLE)
  add_executable(ALL_test)
  target_link_libraries(ALL_test ${PROJECT_NAME}::${PROJECT_NAME})
  set_target_properties(ALL_test PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )
  target_compile_definitions(ALL_test PRIVATE
      ALL_TESTS_AS_SINGLE_PRJ
  )
  foreach(src IN LISTS source_files)
    get_filename_component(name_we ${src} NAME_WE)
    target_sources(ALL_test PRIVATE ${src})
    set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-DTHIS_TEST_FNAME=${name_we}")
  endforeach()
  target_sources(ALL_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/testing.h
    ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.py
    ${CMAKE_SOURCE_DIR}/include/clipp.h
    ${CMAKE_SOURCE_DIR}/include/clipp-narrow.h
    ${CMAKE_SOURCE_DIR}/include/clipp-wide.h
  )
  target_precompile_headers(ALL_test PRIVATE
      "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
  )
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${source_files})
  set_target_properties(ALL_test PROPERTIES FOLDER "Tests")
  source_group("CLIPP" FILES
    ${CMAKE_SOURCE_DIR}/include/clipp.h
    ${CMAKE_SOURCE_DIR}/include/clipp-narrow.h
    ${CMAKE_SOURCE_DIR}/include/clipp-wide.h
  )
  if (WIN32)
      target_compile_definitions(ALL_test PRIVATE
          _WIN32
      )
  endif()
else()
  foreach(src IN LISTS source_files)
    get_filename_component(name_we ${src} NAME_WE)
    add_executable(test-${name_we} ${src})
    target_link_libraries(test-${name_we} ${PROJECT_NAME}::${PROJECT_NAME})
    set_target_properties(test-${name_we} PROPERTIES
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
    )
    add_test(NAME ${name_we} COMMAND $<TARGET_FILE:test-${name_we}>)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src})
    set_target_properties(test-${name_we} PROPERTIES FOLDER "Tests")
    if (WIN32)
        target_compile_definitions(test-${name_we} PRIVATE
            _WIN32
        )
    endif()
  endforeach()
endif()
