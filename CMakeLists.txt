cmake_minimum_required (VERSION 2.8)
project (ImportAuthPlain)

ENABLE_TESTING()

set (ImportAuthPlain_VERSION_MAJOR 1)
set (ImportAuthPlain_VERSION_MINOR 0)

find_package(Threads)

configure_file (
	"${PROJECT_SOURCE_DIR}/ImportAuthPlainConfig.h.in"
	"${PROJECT_BINARY_DIR}/ImportAuthPlainConfig.h"
)
include_directories("${PROJECT_BINARY_DIR}")

set(APP_SOURCES
	main.c
	sqlite3.c
	sqlite3ext.h
	sqlite3.h
)

set(TEST_SOURCES
	CuTest.c
	CuTest.h
	importtest.c
)

set(COMMON_SOURCES
	importplain.c
	importplain.h
)

IF(CMAKE_SYSTEM MATCHES "Linux.*")
  set(PLATFORM_LIBS dl)
ENDIF(CMAKE_SYSTEM MATCHES "Linux.*")

add_executable(importauthplain ${APP_SOURCES} ${COMMON_SOURCES})
target_link_libraries(importauthplain ${CMAKE_THREAD_LIBS_INIT} ${PLATFORM_LIBS})


add_executable(importtest ${TEST_SOURCES} ${COMMON_SOURCES})
add_test(UnitTests importtest)
