set(MFC_EXAMPLES_SOURCES "")

function(add_mfc_example)
    set(flags LINK_IMGUI)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(arg "${flags}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

   # NAME 인자 확인
    if(NOT arg_NAME)
        message(FATAL_ERROR "NAME argument is required")
        return()
    endif()      
    
    # 실행 파일 추가
    add_executable(${arg_NAME} "${arg_SOURCES}" "${CMAKE_CURRENT_SOURCE_DIR}/ThreeContainer.h" "${CMAKE_CURRENT_SOURCE_DIR}/ThreeContainer.cpp")
    target_include_directories(${arg_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries (${arg_NAME} PRIVATE glfw)    
    target_link_libraries (${arg_NAME} PRIVATE threepp::threepp)
    target_compile_options(${arg_NAME} PRIVATE /U_MSC_VER)
    #if(arg_LINK_IMGUI)
    target_link_libraries (${arg_NAME} PRIVATE imgui)        
    #endif()
    set(MFC_EXAMPLE_SOURCES ${MFC_EXAMPLE_SOURCES} ${arg_SOURCES} PARENT_SCOPE)
endfunction()

add_mfc_example(NAME FirstScene SOURCES "FirstSceneExample.h" "FirstSceneExample.cpp")
add_mfc_example(NAME MaterialLight SOURCES "MaterialLight.h" "MaterialLight.cpp")
add_mfc_example(NAME MaterialLightAnimation SOURCES "MaterialLightAnimation.h" "MaterialLightAnimation.cpp")


set(headerFiles
    "ThreeContainer.h"
    "LoadExamples.h"
)
set(sourceFiles
    "ThreeContainer.cpp"
    "LoadExamples.cpp"
)


add_library(mfcexample ${headerFiles} ${sourceFiles} ${MFC_EXAMPLE_SOURCES})
add_library(mfcexample::mfcexample ALIAS mfcexample)
target_include_directories(mfcexample PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries (mfcexample PRIVATE threepp::threepp)
target_link_libraries (mfcexample PRIVATE imgui::imgui)