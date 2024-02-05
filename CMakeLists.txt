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
set(SRC "${CMAKE_CURRENT_LIST_DIR}/src/")

set(HEADERS
	${INCLD}/Main.h
	${INCLD}/Actives.h 
	${INCLD}/AF.h 
	${INCLD}/Array.h 
	${INCLD}/Decodings.h 
	${INCLD}/Encodings.h 
	${INCLD}/LinkedSparseMatrix.h 
	${INCLD}/List.h 
	${INCLD}/ListActives.h 
	${INCLD}/Matrix.h 
	${INCLD}/Reduct.h 
	${INCLD}/SatProblem.h
	${INCLD}/Decodings_cms.h
	${INCLD}/Encodings_cms.h 
	${INCLD}/ExtensionSolver.h 
	${INCLD}/ExternalSolver.h
	${INCLD}/InitialSetSolver.h
	${INCLD}/ScepticalPRCheck.h
	${INCLD}/ScepticalPRParallel.h
	${INCLD}/ScepticalPRSequential.h
	)

set(SRCFILES
    ${SRC}Main.cpp
	${SRC}Actives.c 
	${SRC}AF.c 
	${SRC}Array.c 
	${SRC}Decodings.c 
	${SRC}Encodings.c 
	${SRC}LinkedSparseMatrix.c 
	${SRC}List.c 
	${SRC}ListActives.c 
	${SRC}Matrix.c 
	${SRC}Reduct.c 
	${SRC}SatProblem.c 
	${SRC}Decodings_cms.cpp
	${SRC}Encodings_cms.cpp 
	${SRC}ExtensionSolver.cpp 
	${SRC}ExternalSolver_pstreams.cpp
	${SRC}InitialSetSolver.cpp
	${SRC}ScepticalPRCheck.c
	${SRC}ScepticalPRParallel.cpp
	${SRC}ScepticalPRSequential.cpp
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