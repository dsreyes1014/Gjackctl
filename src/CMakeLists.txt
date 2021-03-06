project (gjackctl C)

cmake_minimum_required (VERSION 3.0)

find_package (PkgConfig REQUIRED)

pkg_check_modules (GTK3 REQUIRED gtk+-3.0)
pkg_check_modules (LIBCONFIG REQUIRED libconfig)
pkg_check_modules (ALSA REQUIRED alsa)
pkg_check_modules (JACK REQUIRED jack)
pkg_check_modules (CLUTTER REQUIRED clutter-1.0)
pkg_check_modules (CLUTTERGTK REQUIRED clutter-gtk-1.0)

include_directories (${GTK3_INCLUDE_DIRS} 
                     ${LIBCONFIG_INCLUDE_DIRS}
                     ${ALSA_INCLUDE_DIRS}
                     ${JACK_INCLUDE_DIRS}
                     ${CLUTTER_INCLUDE_DIRS}
                     ${CLUTTERGTK_INCLUDE_DIRS})

link_directories (${GTK3_LIBRARIES_DIRS}
                  ${LIBCONFIG_LIBRARIES_DIRS}
                  ${ALSA_LIBRARIES_DIRS}
                  ${JACK_LIBRARIES_DIRS}
                  ${CLUTTER_LIBRARIES_DIRS}
                  ${CLUTTERGTK_LIBRARIES_DIRS})

add_executable (gjackctl main.c
                         server_switch.c
                         toggle_rt.c
                         drivers.c
                         alsa_device_names.c
		                 dsp_load.c
                         jack_server_init.c
                         sample_rate.c
                         gjackctl_settings.c
                	     connections.c
                         rt_priority.c
                         server_name.c
                         display.c
                         toggle_no_memlock.c
                         get_arg_vector.c
                         config_file_input.c
                         config_file_init.c
                         toggle_midi.c
                         label_toggle_attr.c
                         port_max.c
                         timeout.c
                         clocksource.c
                         toggle_no_zombies.c
                         toggle_unlock_libs.c
                         period.c
                         frames.c
                         jack_log.c
                         get_jack_gpid.c
                         create_jack_client.c
                         toggle_verbose.c
                         jack_ports.c)

target_link_libraries (gjackctl ${GTK3_LIBRARIES} 
                                ${LIBCONFIG_LIBRARIES}
                                ${ALSA_LIBRARIES}
                                ${JACK_LIBRARIES}
                                ${CLUTTER_LIBRARIES}
                                ${CLUTTERGTK_LIBRARIES})

# Compiler flags
if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "-O2" "-Wall" "-s")        # Optimize
    set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall")       # Show Warnings
    set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_CXX_FLAGS} "-s")  # Strip binary
endif ()

#set (CMAKE_BUILD_TYPE Release)


