CLS

@echo off

set AB1562_LIC_FILE_NAME=1562_node.lic

if defined XTENSA_RG_2019_12_ROOT (
<<<<<<< HEAD
	set XTENSA_EXE_PATH="C:\Airoha_toolchain\RG-2019.12-win32\XtensaTools\bin"
=======
	set XTENSA_EXE_PATH="%XTENSA_RG_2019_12_ROOT%\RG-2019.12-win32\XtensaTools\bin"
>>>>>>> db20e11 (second commit)
	set XTENSA_SYSTEM=%XTENSA_RG_2019_12_ROOT%\RG-2019.12-win32\XtensaTools\config
	set AB1562_LICENSE_FILE_GLOBAL=%XTENSA_RG_2019_12_ROOT%\license\%AB1562_LIC_FILE_NAME%
)else (

<<<<<<< HEAD
	set XTENSA_EXE_PATH=C:\usr\xtensa\XtDevTools\install\tools\RG-2019.12-win32\XtensaTools\bin
	set XTENSA_SYSTEM=C:\usr\xtensa\XtDevTools\XtensaRegistry\RG-2019.12-win32
=======
	set XTENSA_EXE_PATH=C:\Airoha_toolchain\RG-2019.12-win32\XtensaTools\bin
	set XTENSA_SYSTEM=C:\Airoha_toolchain\RG-2019.12-win32
	
>>>>>>> db20e11 (second commit)
)

set PATH=%XTENSA_EXE_PATH%;%PATH%

set XTENSA_CORE=AB1562_i64B_d32B

set AB1562_LICENSE_FILE=%cd%\tools\xtensa\%AB1562_LIC_FILE_NAME%

if not defined AB1562_LICENSE_FILE_GLOBAL goto check_lic_local
if exist %AB1562_LICENSE_FILE_GLOBAL% (
	set LM_LICENSE_FILE=%AB1562_LICENSE_FILE_GLOBAL%
	goto check_lic_end
)

:check_lic_local
if exist %AB1562_LICENSE_FILE% ( 
	set LM_LICENSE_FILE=%AB1562_LICENSE_FILE%
)else (
	echo.
	echo "Warning: Can't find Cadence license file under SDK or xtensa folder, try to use system default setting instead.")	
)

:check_lic_end

set i=0
set target=0
set select1=0
set select2=0
set select3=0
set mode1=0
set rest1=0
set shainfo111=0

set /a "x = 0"
:while1
    echo AB1562 Auto Build Script
    echo 1.Build Debug
    echo 2.Clean
    echo 99.Exit
    echo.
    set /p select1="Select: "
:while2
    IF "%select1%"=="1" (
      set mode1=MODE=DEBUG
      set action=all
:while3
      echo Choose Target Board
      echo 1.AB1562
      echo 2.AB1562M
      echo 3.AB1562A
      echo 4.AB1562AM
      echo 5.AB1562F
      echo 6.AB1561M
      echo 7.AB1562U
      echo 8.AB1563
      echo 9.AB1563A
      echo 99.Exit
      echo.
      set select2=0
      set /p select2="Select: "
          goto fun1
:fun1
          IF "%select2%"=="1" (
            set target=PROJECT=AB1562 PLATFORM=ASIC RF=RF1530C
            echo.
          ) ELSE IF "%select2%"=="2" (
            set target=PROJECT=AB1562M PLATFORM=ASIC RF=RF1530C
            echo.
          ) ELSE IF "%select2%"=="3" (
            set target=PROJECT=AB1562A PLATFORM=ASIC RF=RF1530C
            echo.
          ) ELSE IF "%select2%"=="4" (
            set target=PROJECT=AB1562AM PLATFORM=ASIC RF=RF1530C
            echo.
          ) ELSE IF "%select2%"=="5" (
            set target=PROJECT=AB1562F PLATFORM=ASIC RF=RF1530C
            echo.
          ) ELSE IF "%select2%"=="6" (
            set target=PROJECT=AB1561M PLATFORM=ASIC RF=RF1530C
            echo.
          ) ELSE IF "%select2%"=="7" (
            set target=PROJECT=AB1562U PLATFORM=ASIC RF=RF1530C
            echo.
          ) ELSE IF "%select2%"=="8" (
            set target=PROJECT=AB1563 PLATFORM=ASIC RF=RF1530C
            echo.
		  ) ELSE IF "%select2%"=="9" (
            set target=PROJECT=AB1563A PLATFORM=ASIC RF=RF1530C
            echo.
		  ) ELSE IF "%select2%"=="99" (
            goto :while1
          ) ELSE (
            echo Invalid Select
            goto :while2
          )
          goto :while3
:while3
            echo Choose Application
            echo 1.awsmce_ref_design
            echo 2.headset_ref_design
            echo 99.Exit
            echo.
            set select3=0
            set /p select3="Select: "
            SET STARTTIME=%TIME%
                  goto fun2
:fun2
                  rem generate sha number
                  IF "%shainfo111%"=="1" (
                  goto fun6
                  )

                  set /p ref_head=<.git/HEAD
                  SET _result=%ref_head:~16%
                  set /p sha_number=<.git/refs/heads/%_result%
                  echo const char head_branch[]= "%_result%"; > \n > source/sha_info/sha_number.c
                  echo const char sha_version[]= "%sha_number%"; >> source/sha_info/sha_number.c
                  set shainfo111=1

:fun6
                  set YEAR=%DATE:~0,4%
                  set MONTH=%DATE:~5,2%
                  set DAY=%DATE:~8,2%
                  set CURRENT_DATE=%YEAR%/%MONTH%/%DAY%
                  set HOUR=%TIME:~0,2%
                  set MINUTE=%TIME:~3,2%
                  set SECOND=%TIME:~6,2%
                  set CURRENT_TIME=%HOUR%:%MINUTE%:%SECOND%
                  set BUILD_TIME=%CURRENT_DATE% %CURRENT_TIME%
                  rem echo %CURRENT_DATE% %CURRENT_TIME%

                  echo.    > source/verno/verno.c
                  echo #include "verno.h" >> source/verno/verno.c
                  echo char build_date_time_str[] = "%BUILD_TIME%"; >> source/verno/verno.c
                  echo char sw_verno_str[] = MTK_FW_VERSION; >> source/verno/verno.c
                  echo char hw_verno_str[] = "ab1562_evb"; >> source/verno/verno.c
                  echo.    >> source/verno/verno.c
                  echo #ifdef MTK_SYSLOG_VERSION_2 >> source/verno/verno.c
                  echo #include "memory_attribute.h" >> source/verno/verno.c
                  echo ATTR_LOG_TIMESTAMP log_bin_build_date_time_str[] = "%BUILD_TIME%"; >> source/verno/verno.c
                  echo ATTR_LOG_VERSION log_bin_sw_verno_str[] = MTK_FW_VERSION; >> source/verno/verno.c
                  echo #endif >> source/verno/verno.c
:fun5
                  IF "%select3%"=="1" (
                    xt-make -C .\project\AB1562 %target% %mode1% APP=awsmce_ref_design APPROOTDIR=project/AB1562/apps/mcsync_ref_design %action%
                    echo.
                  ) ELSE IF "%select3%"=="2" (
                    xt-make -C .\project\AB1562 %target% %mode1% APP=headset_ref_design APPROOTDIR=project/AB1562/apps/headset_ref_design %action%
                    echo.
                  ) ELSE IF "%select3%"=="99" (
                    echo.
                    goto :while2
                  ) ELSE (
                    echo Invalid Select
                    echo.
                    goto :while3
                  )
:fun9
            SET ENDTIME=%TIME%
            goto :TimeCalculate
:CompileEnd
            goto :while1

    ) ELSE IF "%select1%"=="2" (
      set mode1=MODE=DEBUG
      set action=clean
      goto :while3
      xt-make -C .\project\AB1562 clean
      echo.
    ) ELSE IF "%select1%"=="99" (
      echo.
      goto :end1
    ) ELSE (
      echo Invalid Select
      echo.
    )
    goto :while1

:TimeCalculate
            rem Change formatting for the start and end times
            for /F "tokens=1-4 delims=:.," %%a in ("%STARTTIME%") do (
               set /A "TIME1=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
            )

            for /F "tokens=1-4 delims=:.," %%a in ("%ENDTIME%") do (
               set /A "TIME2=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
            )
            rem Calculate the elapsed time by subtracting values
            set /A elapsed=TIME2-TIME1

            rem Format the results for output
            set /A hh=elapsed/(60*60*100)
            set /A rest1=elapsed%%(60*60*100), mm=rest1/(60*100), rest1%%=60*100, ss=rest1/100, cc=rest1%%100

            if %hh% lss 10 set hh=0%hh%
            if %mm% lss 10 set mm=0%mm%
            if %ss% lss 10 set ss=0%ss%
            if %cc% lss 10 set cc=0%cc%

            set DURATION=%hh%:%mm%:%ss%,%cc%

            echo Start    : %STARTTIME%
            echo Finish   : %ENDTIME%
            echo          ---------------
            echo Duration : %DURATION%
            echo.
            goto :CompileEnd


:whiletest
      echo Choose Target Board
      echo 1.AB1562_EVB
      echo 99.Exit
      echo.
      set select2=0
      set /p select2="Select: "
          goto fun3
:fun3
          IF "%select2%"=="1" (
            set target=PROJECT=AB1562 PLATFORM=FPGA RF=RF1530C
            echo.

          ) ELSE IF "%select2%"=="99" (
            goto :while1
          ) ELSE (
            echo Invalid Select
            goto :while2
          )
          goto :whileTestApp
:whileTestApp
            echo Choose Test
            echo 1.dvt_demo
            echo 2.SLT
            echo 99.Exit
            echo.
            SET STARTTIME=%TIME%
            set select3=0
            set /p select3="Select: "
                  goto fun4
:fun4
                  IF "%select3%"=="1" (
                    xt-make -C .\project\AB1562 %target% %mode1% APP=dvt_demo APPROOTDIR=project/AB1562/test_loads/DVT/peripheral/dvt_demo %action%
                    echo.
                  ) ELSE IF "%select3%"=="2" (
                    xt-make -C .\project\AB1562 %target% %mode1% APP=slt APPROOTDIR=project/AB1562/test_loads/slt TARGET=SLT %action%
                    echo.
                  ) ELSE IF "%select3%"=="99" (
                    echo.
                    goto :whiletest
                  ) ELSE (
                    echo Invalid Select
                    echo.
                    goto :whileTestApp
                  )
            SET ENDTIME=%TIME%
            goto :TimeCalculate

:end1
