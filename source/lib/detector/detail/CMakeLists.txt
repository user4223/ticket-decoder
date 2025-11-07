PROJECT(ticket-decoder-detector-detail)

IF (WITH_SQUARE_DETECTOR)
    add_subdirectory(square)
ENDIF()

IF (WITH_CLASSIFIER_DETECTOR)
    add_subdirectory(classifier)
ENDIF()
