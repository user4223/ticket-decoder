PROJECT(ticket-decoder-interpreter-detail)

add_subdirectory(common)
add_subdirectory(uic918)
add_subdirectory(sbb)
add_subdirectory(vdv)
if (WITH_SIGNATURE_VERIFIER)
    add_subdirectory(verifier)
ENDIF()
