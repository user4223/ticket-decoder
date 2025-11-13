PROJECT(ticket-decoder-lib)

add_subdirectory(infrastructure)

add_subdirectory(io)
add_subdirectory(decoder)
add_subdirectory(dip)
add_subdirectory(detector)

add_subdirectory(interpreter)
add_subdirectory(utility)

IF (WITH_TICKET_ANALYZER)
    add_subdirectory(ui)
ENDIF()

add_subdirectory(api)
