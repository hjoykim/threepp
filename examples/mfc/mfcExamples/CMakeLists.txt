set(sourceFolder mfcExamples)
file(GLOB headerFiles CONFIGURE_DEPENDS
	*.h
	*.hpp
)	

file(GLOB sourceFiles CONFIGURE_DEPENDS
	*.c
	*.cpp
)	

add_library(mfcexample ${headerFiles} ${sourceFiles})
add_library(mfcexample::mfcexample ALIAS mfcexample)
target_include_directories(mfcexample PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries (mfcexample PRIVATE threepp::threepp)
target_link_libraries (mfcexample PRIVATE imgui::imgui)