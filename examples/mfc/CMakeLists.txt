if (THREEPP_MFC)
project(mfcapp)
set(CMAKE_MFC_FLAG 2)
include(CMakeDependentOption)

add_subdirectory(mfcexamples)

set(sourceFolder src)
file(GLOB headerFiles CONFIGURE_DEPENDS
	${sourceFolder}/*.h*
)	

file(GLOB sourceFiles CONFIGURE_DEPENDS
	${sourceFolder}/*.c*
)	

file(GLOB resourceFiles CONFIGURE_DEPENDS
	${sourceFolder}/*.rc
	${sourceFolder}/res/*.rc2
	${sourceFolder}/res/*.ico
	${sourceFolder}/res/*.bmp	
	${sourceFolder}/res/ribbon.mfcribbon-ms	
)
set(SOURCES ${sourceFiles})
set(HEADERS ${headerFiles})
set(RESOURCE_FILES ${resourceFiles})

source_group("Source Files" FILES ${sourceFiles})
source_group("Header Files" FILES ${headerFiles})
source_group("Resources" FILES ${resourceFiles})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${sourceFolder} ${threepp_SOURCE_DIR}/src/external/glad)
target_link_libraries (${PROJECT_NAME} PRIVATE threepp)
target_link_libraries (${PROJECT_NAME} PRIVATE mfcexample)
target_link_libraries (${PROJECT_NAME} PRIVATE imgui::imgui)
target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${sourceFolder}/pch.h)
target_link_libraries(${PROJECT_NAME} PRIVATE
	user32
	gdi32
	comctl32
	ole32
	oleaut32
	uuid
	winmm
	advapi32
	opengl32
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	LINK_FLAGS "/SUBSYSTEM:WINDOWS /NODEFAULTLIB:LIBCMTD"
)

endif()