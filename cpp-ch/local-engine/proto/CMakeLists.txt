# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
file(GLOB protobuf_files
        substrait/*.proto
        substrait/extensions/*.proto
        )

FOREACH(FIL ${protobuf_files})
    file(RELATIVE_PATH FIL_RELATIVE ${ClickHouse_SOURCE_DIR}/utils/extern-local-engine/proto/ ${FIL})
    string(REGEX REPLACE "\\.proto" "" FILE_NAME ${FIL_RELATIVE})
    LIST(APPEND SUBSTRAIT_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.pb.cc")
    LIST(APPEND SUBSTRAIT_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.pb.h")
ENDFOREACH()

add_custom_target(
        generate_substrait
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../contrib/google-protobuf-cmake/protoc -I${CMAKE_CURRENT_SOURCE_DIR} -I${ClickHouse_SOURCE_DIR}/contrib/google-protobuf/src --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/ ${protobuf_files}
        DEPENDS protoc
        COMMENT "Running cpp protocol buffer compiler"
        VERBATIM )

set_source_files_properties(${SUBSTRAIT_SRCS} PROPERTIES GENERATED TRUE)

add_library(substrait ${SUBSTRAIT_SRCS})
add_dependencies(substrait generate_substrait)
target_compile_options(substrait PUBLIC -fPIC -Wno-reserved-identifier -Wno-deprecated -Wno-shadow-field)
target_include_directories(substrait SYSTEM BEFORE PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(substrait ch_contrib::protobuf)

