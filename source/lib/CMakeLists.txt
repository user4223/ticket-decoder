PROJECT(ticket-decoder-lib)

add_subdirectory(io)
add_subdirectory(barcode)
add_subdirectory(dip)

add_subdirectory(uic918/u_flex/v1.3)
add_subdirectory(uic918/u_flex/v3.0)

add_subdirectory(uic918)
add_subdirectory(utility)

IF (WITH_TICKET_ANALYZER)
    add_subdirectory(ui)
ENDIF()

add_subdirectory(api)
