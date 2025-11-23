PROJECT(ticket-decoder-lib)

add_subdirectory(infrastructure)
add_subdirectory(input)
add_subdirectory(output)
add_subdirectory(decoder)
add_subdirectory(detector)
add_subdirectory(interpreter)
add_subdirectory(dip)
add_subdirectory(utility)

IF (WITH_TICKET_ANALYZER)
    add_subdirectory(ui)
ENDIF()

add_subdirectory(api)
