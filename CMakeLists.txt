cmake_minimum_required(VERSION 3.15)
project(csp_study LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall)
add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

# 辅助函数：为一道题创建 solution / brute / gen 三个 target
function(add_problem TARGET_PREFIX PROBLEM_DIR)
    set(SOL "${PROBLEM_DIR}/solution.cpp")
    set(BRT "${PROBLEM_DIR}/brute.cpp")
    set(GEN "${PROBLEM_DIR}/gen.cpp")

    if(EXISTS "${CMAKE_SOURCE_DIR}/${SOL}")
        add_executable(${TARGET_PREFIX} ${SOL})
    endif()
    if(EXISTS "${CMAKE_SOURCE_DIR}/${BRT}")
        add_executable(${TARGET_PREFIX}_brute ${BRT})
    endif()
    if(EXISTS "${CMAKE_SOURCE_DIR}/${GEN}")
        add_executable(${TARGET_PREFIX}_gen ${GEN})
    endif()
endfunction()

# ── CSP 官方题目 ──
# 路径格式：problems/YYYYMM/tN-题目名/solution.cpp
file(GLOB CSP_SOLUTIONS RELATIVE ${CMAKE_SOURCE_DIR}
    problems/*/t*/solution.cpp)

foreach(SOL ${CSP_SOLUTIONS})
    string(REGEX MATCH "problems/([0-9]+)/(t[0-9]+)" _m ${SOL})
    if(_m)
        set(SESSION ${CMAKE_MATCH_1})
        set(TNUM ${CMAKE_MATCH_2})
        get_filename_component(DIR ${SOL} DIRECTORY)
        add_problem("csp_${SESSION}_${TNUM}" ${DIR})
    endif()
endforeach()

# ── 洛谷题目 ──
# 路径格式：problems/luogu/PXXXX-题目名/solution.cpp
file(GLOB LUOGU_SOLUTIONS RELATIVE ${CMAKE_SOURCE_DIR}
    problems/luogu/*/solution.cpp)

foreach(SOL ${LUOGU_SOLUTIONS})
    get_filename_component(DIR ${SOL} DIRECTORY)
    get_filename_component(DIRNAME ${DIR} NAME)
    string(REGEX MATCH "^(P[0-9]+)" _m ${DIRNAME})
    if(_m)
        add_problem(${CMAKE_MATCH_1} ${DIR})
    endif()
endforeach()
