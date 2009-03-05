# Microsoft Developer Studio Project File - Name="KGEventView" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=KGEventView - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KGEventView.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KGEventView.mak" CFG="KGEventView - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KGEventView - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KGEventView - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KGEventView - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "__NO_DLL_IMPL__" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /libpath:"lib"

!ELSEIF  "$(CFG)" == "KGEventView - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__NO_DLL_IMPL__" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"lib"

!ENDIF 

# Begin Target

# Name "KGEventView - Win32 Release"
# Name "KGEventView - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CheckComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\commontool.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogRun.cpp
# End Source File
# Begin Source File

SOURCE=.\EditTxt.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoRaw.cpp
# End Source File
# Begin Source File

SOURCE=.\KGEventView.cpp
# End Source File
# Begin Source File

SOURCE=.\KGEventView.rc
# End Source File
# Begin Source File

SOURCE=.\KGEventViewDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\KGEventViewView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadAPIDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadEvtDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\SAStatusLog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\src\xkexception.cpp
# End Source File
# Begin Source File

SOURCE=.\src\xkhostinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\xkmsgqueue.cpp
# End Source File
# Begin Source File

SOURCE=.\src\xknetunit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\xksocket.cpp
# End Source File
# Begin Source File

SOURCE=.\src\xkthread.cpp
# End Source File
# Begin Source File

SOURCE=.\src\xkthreadqueue.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CheckComboBox.h
# End Source File
# Begin Source File

SOURCE=.\commontool.h
# End Source File
# Begin Source File

SOURCE=.\DialogRun.h
# End Source File
# Begin Source File

SOURCE=.\EditTxt.h
# End Source File
# Begin Source File

SOURCE=.\InfoDetail.h
# End Source File
# Begin Source File

SOURCE=.\InfoRaw.h
# End Source File
# Begin Source File

SOURCE=.\KGEventView.h
# End Source File
# Begin Source File

SOURCE=.\KGEventViewDoc.h
# End Source File
# Begin Source File

SOURCE=.\KGEventViewView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MyDlgBar.h
# End Source File
# Begin Source File

SOURCE=.\ReadAPIDetail.h
# End Source File
# Begin Source File

SOURCE=.\ReadEvtDetail.h
# End Source File
# Begin Source File

SOURCE=.\ReadPacket.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SAStatusLog.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\inc\xkdef.h
# End Source File
# Begin Source File

SOURCE=.\inc\xkexception.h
# End Source File
# Begin Source File

SOURCE=.\inc\xkhostinfo.h
# End Source File
# Begin Source File

SOURCE=.\inc\xkmsgdef.h
# End Source File
# Begin Source File

SOURCE=.\inc\xkmsgqueue.h
# End Source File
# Begin Source File

SOURCE=.\inc\xknetunit.h
# End Source File
# Begin Source File

SOURCE=.\inc\xksocket.h
# End Source File
# Begin Source File

SOURCE=.\inc\xkthread.h
# End Source File
# Begin Source File

SOURCE=.\inc\xkthreadqueue.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\buttondown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonfocus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\KGEventView.ico
# End Source File
# Begin Source File

SOURCE=.\res\KGEventView.rc2
# End Source File
# Begin Source File

SOURCE=.\res\KGEventViewDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree_ico.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
