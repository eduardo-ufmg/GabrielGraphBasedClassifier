# GabrielGraphBasedClassifiers/nn

# Common sources for NN components
set(NN_COMMON_SOURCES
    
)

# NN-label executable
if(BUILD_NN_LABEL)
    add_executable(nn-label
        label/label.cpp
        label/nearestSVlabel.cpp
        ${NN_COMMON_SOURCES}
    )
    target_link_libraries(nn-label common)
    target_include_directories(nn-label PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# NN-train executable
if(BUILD_NN_TRAIN)
    add_executable(nn-train
        train/train.cpp
        train/computeSVs.cpp
        ${NN_COMMON_SOURCES}
    )
    target_link_libraries(nn-train common)
    target_include_directories(nn-train PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()