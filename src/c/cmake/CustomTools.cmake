# Makro içinde yapılan değişiklikler doğrudan çağıran ana dosyayı etkiler
macro(filter_existing_files INPUT_LIST_NAME OUTPUT_LIST_NAME)
    set(TEMPORARY_VALID_LIST "")

    foreach(file IN LISTS ${INPUT_LIST_NAME})
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${file}")
            message(STATUS "Source file verified: src/${file}")
            list(APPEND TEMPORARY_VALID_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/${file}")
        else()
            message(WARNING "Critical file is missing: src/${file} - Removing from build list!")
        endif()
    endforeach()

    # PARENT_SCOPE kullanmadan doğrudan çıktı değişkenine atama yapıyoruz
    set(${OUTPUT_LIST_NAME} ${TEMPORARY_VALID_LIST})
    # set(${OUTPUT_LIST_NAME} ${TEMPORARY_VALID_LIST} PARENT_SCOPE) function olsaydı kullanılırdı
endmacro()