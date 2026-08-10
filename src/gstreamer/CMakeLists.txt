cmake_minimum_required(VERSION 3.0)
project(gstreamer)

find_package(PkgConfig REQUIRED)
find_package(GTK REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER_AUDIO REQUIRED gstreamer-audio-1.0)
pkg_check_modules(GSTREAMER_PBUTILS REQUIRED gstreamer-pbutils-1.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

set(BASIC_PATH "${PROJECT_SOURCE_DIR}/basic/")
set(PLAYBACK_PATH "${PROJECT_SOURCE_DIR}/playback/")

include_directories( # ${GTK3_INCLUDE_DIRS}
                    ${GSTREAMER_INCLUDE_DIRS}
                    ${GSTREAMER_AUDIO_INCLUDE_DIRS}
                    ${GSTREAMER_PBUTILS_INCLUDE_DIRS})

link_directories( # ${GTK3_LIBRARY_DIRS}
                 ${GSTREAMER_LIBRARY_DIRS}
                 ${GSTREAMER_AUDIO_LIBRARY_DIRS}
                 ${GSTREAMER_PBUTILS_LIBRARY_DIRS})

set(BASIC_TUTORIAL_SOURCES
        hello_world.c 
        base_create_pipeline.c
        dynamic_pipelines.c
        time_management.c
        # gui.c
        media_formats_and_pad.c
        multithreading_and_pad_availablity.c
        short_cutting_pipeline.c
        media_info.c
        network-resilient.c
        playback_speed.c
)

set(PLAYBACK_TUTORIAL_SOURCES 
        playbin_usage.c
        subtitle_management.c 
        short_cutting_pipeline.c
        progressive_streaming.c 
        color_balance.c 
        audio_visualization.c
        custom_playbin_sinks.c
    )


foreach(TUTORIAL_SOURCE ${BASIC_TUTORIAL_SOURCES})
    get_filename_component(TUTORIAL_NAME ${TUTORIAL_SOURCE} NAME_WE)
    add_executable(${TUTORIAL_NAME} ${BASIC_PATH}/${TUTORIAL_SOURCE})
    target_link_libraries(${TUTORIAL_NAME} ${GSTREAMER_LIBRARIES} ${GTK3_LIBRARIES} 
                                ${GSTREAMER_AUDIO_LIBRARIES} ${GSTREAMER_PBUTILS_LIBRARIES})
endforeach()

foreach(TUTORIAL_SOURCE ${PLAYBACK_TUTORIAL_SOURCES})
    get_filename_component(TUTORIAL_NAME ${TUTORIAL_SOURCE} NAME_WE)
    add_executable(playback_${TUTORIAL_NAME} ${PLAYBACK_PATH}/${TUTORIAL_SOURCE})
    target_link_libraries(playback_${TUTORIAL_NAME} ${GSTREAMER_LIBRARIES} ${GTK3_LIBRARIES} 
                                ${GSTREAMER_AUDIO_LIBRARIES} ${GSTREAMER_PBUTILS_LIBRARIES})
endforeach()

# add_executable(test_rtsp test/test_rtsp.c)
# target_link_libraries(test_rtsp ${GSTREAMER_LIBRARIES} ${GTK3_LIBRARIES} 
#                         ${GSTREAMER_AUDIO_LIBRARIES} ${GSTREAMER_PBUTILS_LIBRARIES})

# add_executable(main test/main.c)
# target_link_libraries(main ${GSTREAMER_LIBRARIES} ${GTK3_LIBRARIES} 
#                         ${GSTREAMER_AUDIO_LIBRARIES} ${GSTREAMER_PBUTILS_LIBRARIES})