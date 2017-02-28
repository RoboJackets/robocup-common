function(PROTOBUF_GENERATE_NANOPB_CPP SRCS HDRS)
  if(NOT ARGN)
    message(SEND_ERROR "Error: PROTOBUF_GENERATE_NANOPB_CPP() called without any proto files")
    return()
  endif()

  if(PROTOBUF_GENERATE_CPP_APPEND_PATH) # This variable is common for all types of output.
    # Create an include path for each file specified
    foreach(FIL ${ARGN})
      get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
      get_filename_component(ABS_PATH ${ABS_FIL} PATH)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  else()
    set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  if(DEFINED PROTOBUF_IMPORT_DIRS)
    foreach(DIR ${Protobuf_IMPORT_DIRS})
      get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  endif()

  set(${SRCS})
  set(${HDRS})
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)

    list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/nanopb/${FIL_WE}.pb.c")
    list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/nanopb/${FIL_WE}.pb.h")

    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/nanopb/${FIL_WE}.pb.c"
             "${CMAKE_CURRENT_BINARY_DIR}/nanopb/${FIL_WE}.pb.h"
      COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
      ARGS --nanopb_out=${CMAKE_CURRENT_BINARY_DIR}/nanopb
           --plugin=protoc-gen-nanopb=${NANOPB_CPP_PLUGIN}
           ${_protobuf_include_path} ${ABS_FIL}
      DEPENDS ${ABS_FIL} ${Protobuf_PROTOC_EXECUTABLE} libprotobuf-nanopb
      COMMENT "Running nanopb C++ protocol buffer compiler on ${FIL}"
      VERBATIM)
  endforeach()

  set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${SRCS} ${${SRCS}} PARENT_SCOPE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/nanopb/)

# positon-independent-code flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
protobuf_generate_cpp(PROTO_SRC PROTO_INCLUDE ${PROTO_FILES})
PROTOBUF_GENERATE_NANOPB_CPP(PROTO_NANOPB_SRC PROTO_NANOPB_INCLUDE ${PROTO_FILES})

add_library(proto_messages STATIC ${PROTO_SRC} ${PROTO_INCLUDE} ${PROTO_NANOPB_SRC} ${PROTO_NANOPB_INCLUDE})
add_library(nanopb_messages STATIC ${PROTO_NANOPB_SRC} ${PROTO_NANOPB_INCLUDE})
target_link_libraries(nanopb_messages libprotobuf-nanopb)
target_link_libraries(proto_messages ${PROTOBUF_LIBRARIES})