cmake_minimum_required(VERSION 3.17)

project(http_microservice_tests LANGUAGES C CXX)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)## Always include the current directory

find_package(OpenSSL 3.1 REQUIRED)
if (OPENSSL_FOUND)
    message(STATUS, "OPENSSL Version -> ${OPENSSL_VERSION}")
    message(STATUS, "OPENSSL INC DiR -> ${OPENSSL_INCLUDE_DIR}")
    message(STATUS, "OPENSSL LIBRARY -> ${OPENSSL_LIBRARIES}")
endif()

find_package(CURL REQUIRED)
if (CURL_FOUND)
    message(STATUS, "CURL VERSION -> ${CURL_VERSION_STRING}")
    message(STATUS, "CURL LIBRARIES -> ${CURL_LIBRARIES}")
    message(STATUS, "CURL INC DIR -> ${CURL_INCLUDE_DIR}")
endif()

find_package(cpprestsdk CONFIG REQUIRED)
if (cpprestsdk_FOUND)
    message(STATUS, "FOUND cpprestsdk")
endif()

# Specify the Solidity compiler
find_program(SOLC solc)
if(NOT SOLC)
    message(FATAL_ERROR "solc not found. Please install the Solidity compiler.")
endif()
message(STATUS, "Found solc -> ${SOLC}")

# Define the source and output directories
set(SOLIDITY_SRC_DIR "${CMAKE_SOURCE_DIR}/contracts")
set(SOLIDITY_BUILD_DIR "${CMAKE_BINARY_DIR}/contracts")

# Create the output directory
file(MAKE_DIRECTORY ${SOLIDITY_BUILD_DIR})

# Function to compile a Solidity file
function(compile_solidity SOURCE_FILE)
    get_filename_component(FILE_NAME ${SOURCE_FILE} NAME_WE)
    set(OUTPUT_FILE "${SOLIDITY_BUILD_DIR}/${FILE_NAME}.bin")
    set(ABI_FILE "${SOLIDITY_BUILD_DIR}/${FILE_NAME}.abi")

    add_custom_command(
        OUTPUT ${OUTPUT_FILE} ${ABI_FILE}
        --base-path . --include-path ./node_modules
        COMMAND ${SOLC} --base-path ${CMAKE_SOURCE_DIR}/contracts --include-path ${CMAKE_SOURCE_DIR}/contracts/node_modules --allow-paths ${CMAKE_SOURCE_DIR}/contracts/node_modules ${SOURCE_FILE} --bin --abi --optimize -o ${SOLIDITY_BUILD_DIR} --overwrite
        DEPENDS ${SOURCE_FILE}
        COMMENT "Compiling ${SOURCE_FILE}..."
    )

    add_custom_target(${FILE_NAME}_target ALL DEPENDS ${OUTPUT_FILE} ${ABI_FILE})
endfunction()

# Collect all Solidity source files
file(GLOB SOLIDITY_SOURCES "${SOLIDITY_SRC_DIR}/*.sol")

# Compile each Solidity source file
foreach(SOURCE_FILE ${SOLIDITY_SOURCES})
    compile_solidity(${SOURCE_FILE})
endforeach()

add_executable(http_server ${CMAKE_CURRENT_SOURCE_DIR}/main.cc 
${CMAKE_CURRENT_SOURCE_DIR}/interrupt_handler.h  ${CMAKE_CURRENT_SOURCE_DIR}/interrupt_handler.cc)
target_include_directories(http_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tools/toml11/include)
target_link_libraries(http_server PRIVATE OpenSSL::Crypto CURL::libcurl cpprestsdk::cpprest)

add_executable(http_client_test ${CMAKE_CURRENT_SOURCE_DIR}/main_client.cc)
target_include_directories(http_client_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(http_client_test PRIVATE OpenSSL::Crypto CURL::libcurl cpprestsdk::cpprest)

add_executable(abi_loading_test ${CMAKE_CURRENT_SOURCE_DIR}/test_abi_loading.cc)
target_include_directories(abi_loading_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(abi_loading_test PRIVATE OpenSSL::Crypto CURL::libcurl cpprestsdk::cpprest)
