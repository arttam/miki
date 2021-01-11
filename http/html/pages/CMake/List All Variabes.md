## List all assigned CMake variables

```bash
cmake -LAH ../build
```

## List all CMake variables defined and their values
```
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
```

## Or filtered out
```
get_cmake_property(_variableNames VARIABLES)                     
list (SORT _variableNames)                                       
foreach (_variableName ${_variableNames})                        
    unset(MATCHED)                                               
    string(REGEX MATCH "Qt5" MATCHED ${_variableName})           
    if (NOT MATCHED)
        continue()     
    endif()            
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
```