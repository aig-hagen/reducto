cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ParallelSolver" LANGUAGES C CXX)

# Add source to this project's executable.
set(INCLD "${CMAKE_CURRENT_LIST_DIR}/include")
set(INCLD_LOGIC "${INCLD}/logic")
set(INCLD_UTIL "${INCLD}/util")
set(SRC "${CMAKE_CURRENT_LIST_DIR}/src/")
set(SRC_LOGIC "${SRC}logic/")
set(SRC_UTIL "${SRC}util/")

set(HEADERS
	${INCLD}/Main.h
	${INCLD_LOGIC}/Actives.h 
	${INCLD_LOGIC}/AF.h 
	${INCLD_LOGIC}/Decodings.h 
	${INCLD_LOGIC}/Encodings.h 
	${INCLD_LOGIC}/ListActives.h 
	${INCLD_LOGIC}/Reduct.h 
	${INCLD_LOGIC}/SatProblem.h
	${INCLD_LOGIC}/Decodings_cms.h
	${INCLD_LOGIC}/Encodings_cms.h 
	${INCLD_LOGIC}/ExtensionSolver.h 
	${INCLD_LOGIC}/ExternalSolver.h
	${INCLD_LOGIC}/InitialSetSolver.h
	${INCLD_LOGIC}/Parser_iccma.h
	${INCLD_LOGIC}/ScepticalPRCheck.h
	${INCLD_LOGIC}/ScepticalPRParallel.h
	${INCLD_LOGIC}/ScepticalPRSequential.h
	${INCLD_LOGIC}/Tests.h

	${INCLD_UTIL}/Array.h 
	${INCLD_UTIL}/LinkedSparseMatrix.h 
	${INCLD_UTIL}/List.h 
	${INCLD_UTIL}/Matrix.h
	${INCLD_UTIL}/MemoryWatchDog.h 
	${INCLD_UTIL}/Enums.h
	)

set(SRCFILES
    ${SRC}Main.cpp
	${SRC_LOGIC}Actives.c 
	${SRC_LOGIC}AF.c 
	${SRC_LOGIC}Decodings.c 
	${SRC_LOGIC}Encodings.c 
	${SRC_LOGIC}ListActives.c 
	${SRC_LOGIC}Reduct.c 
	${SRC_LOGIC}SatProblem.c 
	${SRC_LOGIC}Decodings_cms.cpp
	${SRC_LOGIC}Encodings_cms.cpp 
	${SRC_LOGIC}ExtensionSolver.cpp 
	${SRC_LOGIC}ExternalSolver_pstreams.cpp
	${SRC_LOGIC}InitialSetSolver.cpp
	${SRC_LOGIC}Parser_iccma.cpp
	${SRC_LOGIC}ScepticalPRCheck.c
	${SRC_LOGIC}ScepticalPRParallel.cpp
	${SRC_LOGIC}ScepticalPRSequential.cpp
	${SRC_LOGIC}Tests.cpp

	${SRC_UTIL}Array.c
	${SRC_UTIL}LinkedSparseMatrix.c 
	${SRC_UTIL}List.c
	${SRC_UTIL}Matrix.c
	${SRC_UTIL}MemoryWatchDog.c
	${SRC_UTIL}Enums.cpp
	)

set(SOURCES ${SRCFILES} ${HEADERS})
add_executable(ParallelSolver ${SOURCES})

target_include_directories(ParallelSolver PUBLIC INCLD)

# Add libraries

# --- OpenMP ---
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(ParallelSolver PUBLIC OpenMP::OpenMP_CXX)
endif()

if(OpenMP_C_FOUND)
    target_link_libraries(ParallelSolver PUBLIC OpenMP::OpenMP_C)
endif()

# --- pstreams ---
set(LIB_PSTREAMS "${CMAKE_CURRENT_LIST_DIR}/lib/pstreams-1.0.3")

add_subdirectory(${LIB_PSTREAMS})
target_link_libraries(ParallelSolver PUBLIC PStreams)
target_include_directories(ParallelSolver PUBLIC 
	"${PROJECT_BINARY_DIR}"
	"${LIB_PSTREAMS}"
)

# --- cryptominisat ---

set(LIB_CMS "${CMAKE_CURRENT_LIST_DIR}/lib/cryptominisat-5.11.15")

add_subdirectory(${LIB_CMS})
target_link_libraries(ParallelSolver PUBLIC cryptominisat5)
target_include_directories(ParallelSolver PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${LIB_CMS}"
)


# further settings

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ParallelSolver PROPERTY CXX_STANDARD 20)
endif()