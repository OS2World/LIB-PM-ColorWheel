# test.mak
# Creato dal MakeMake del WorkFrame/2 IBM alle 18:16:20 del 28 Gennaio 1998
#
# Le azioni incluse in questo file Make sono:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .c .obj .rc .res 

.all: \
    .\test.exe

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

{F:\WORKDESK\Progetti\customcontrol\colorwheel}.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ss /Q /V"(c) 1997 * Alessandro Cantatore" /Oc /Rn /Gu /Ol /C %s

{F:\WORKDESK\Progetti\customcontrol\colorwheel}.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ss /Q /V"(c) 1997 * Alessandro Cantatore" /Oc /Rn /Gu /Ol /C %s

.\test.exe: \
    .\test.obj \
    .\test.res \
    {$(LIB)}editcol.lib
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe @<<
     /B" /exepack:2 /pmtype:pm /packd /optfunc /nologo"
     /Fetest.exe 
     editcol.lib 
     .\test.obj
<<
    rc.exe -x2 .\test.res test.exe

.\test.res: \
    F:\WORKDESK\Progetti\customcontrol\colorwheel\test.rc \
    {F:\WORKDESK\Progetti\customcontrol\colorwheel;$(INCLUDE)}TEST.ICO

.\test.obj: \
    F:\WORKDESK\Progetti\customcontrol\colorwheel\test.c \
    {F:\WORKDESK\Progetti\customcontrol\colorwheel;$(INCLUDE);}editcol.h
