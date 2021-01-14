cmake_minimum_required(VERSION 3.10.0)
project(mage)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  # Use MFC
  add_definitions(-D_AFXDLL)
  # Use static MFC libraries
  set(CMAKE_MFC_FLAG 1)

  # We need to compile as C++ to get MFC working
  file(GLOB_RECURSE CFILES "${CMAKE_SOURCE_DIR}/*.c")
  SET_SOURCE_FILES_PROPERTIES(${CFILES} PROPERTIES LANGUAGE CXX)
else ()
  find_package(Motif REQUIRED)
  find_package(X11 REQUIRED)
  include_directories(${MOTIF_INCLUDE_DIR})
endif()

set(mage_SOURCES
  MAGEANGL.c
  MAGEBALL.c
  MAGEBBOX.c
  MAGECLON.c
  MAGECOLR.c
  MAGEDLOG.c
  MAGEDRAW.c
  MAGEFILE.c
  MAGEFOO.c
  MAGEHELP.c
  MAGEINIT.c
  MAGEINPT.c
  MAGELIST.c
  MAGEMENU.c
  MAGEOUT.c
  MAGEOUTR.c
  MAGEPIXL.c
  MAGEPOST.c
  MAGEREND.c
  MAGESCOR.c
  MAGESCRT.c
  MAGESETS.c
  MAGESYNC.c
  MAGETABL.c
  MAGEUTIL.c
  MUXMBAR.c
  MUXMBBOX.c
  MUXMCOLR.c
  MUXMDLOG.c
  MUXMDRAW.c
  MUXMFILE.c
  MUXMINIT.c
  MUXMMAIN.c
  MUXMMENU.c
  MUXMOUT.c
  MUXMPIPE.c
  MUXMTABL.c
  MUXMWIND.c
)

set(mage_HEADERS
  MAGEANGL.h
  MAGEBALL.h
  MAGEBBOX.h
  MAGECLON.h
  MAGECMYK.h
  MAGECOLR.h
  MAGEDLOG.h
  MAGEFLAG.h
  MAGEFOO.h
  MAGE.h
  MAGEhdr.h
  MAGEHELP.h
  MAGELIST.h
  MAGEMENU.h
  MAGEOUTR.h
  MAGEPOST.h
  MAGEREND.h
  MAGESCRT.h
  MAGESYNC.h
  MAGETABL.h
)

add_executable(mage ${mage_SOURCES} ${mage_HEADERS})
if (NOT WIN32)
  if (APPLE)
    target_link_libraries(mage ${MOTIF_LIBRARIES} X11::Xt X11::X11 m)
  else()
    # Not sure why the above does not work on Ubuntu 20.04
    target_link_libraries(mage ${MOTIF_LIBRARIES} Xt X11 m)
  endif()
endif()

