PROJECT(ticket-decoder-detector-detail)

add_subdirectory(common)

IF (WITH_SQUARE_DETECTOR)
    add_subdirectory(square)
ENDIF()

IF (WITH_CLASSIFIER_DETECTOR)
    add_subdirectory(classifier)
ENDIF()
