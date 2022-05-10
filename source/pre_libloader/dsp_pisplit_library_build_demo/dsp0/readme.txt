This demo for how to build a DSP0 PIC library.
On this demo project:
    1. Define * _start() function: this function is a Key function of the PIC lib.
    2. Prepare the export_parameter_array[] : for the PIC lib export its entries to the main program.
        Let main program can call the PIC lib functions after the PIC lib load done, like: dsp0_demo_lib_entry_1().
        There are three lib entries need export to main program:
            dsp0_demo_lib_entry_1() and dsp0_demo_lib_entry_2() defined in source file of lib_main.c
            dsp0_demo_lib_entry_3 defined in a static sub-lib of sub_lib_demo.a.
    3. Prepare the import_parameter_array[] : for the main program can import some service function to the PIC lib.
        Let the PIC lib can call some main program basic service after the PIC lib load done, like : printf().
    4. Also sample to user: how to build a sub-lib for PIC lib.
        Like: 
            A. The function of dsp0_demo_lib_entry_3() defined in a sub-lib. The sub-lib is a static lib for the PIC lib.
                So user also create and build the sub-lib of sub_lib_demo.a firstly.
		In the dsp0 folder, the copy of dsp0_sub_lib_demo.a comes from the dsp0_sub_lib_for_PIC folder.
                please reference the sub-lib build demo of dsp\project\mt2811a_evb\templates\dsp_pisplit_library_build_demo\dsp0_sub_lib_for_PIC.
                    dsp\project\mt2811a_evb\templates\dsp_pisplit_library_build_demo\dsp0_sub_lib_for_PIC\readme.txt had a detail comments.
            B. Then link the dsp0_sub_lib_demo.a to the PIC lib.
                please reference the file of dsp\project\mt2811a_evb\templates\dsp_pisplit_library_build_demo\dsp0\Makefile line 73.
