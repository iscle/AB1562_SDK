#!/usr/bin/python3
# -*- coding: utf-8 -*-

# python3 footprint_gen.py footprint_gen_mcu.cfg mapfile footprint_mcu.log
# python3 footprint_gen.py footprint_gen_dsp.cfg mapfile footprint_dsp.log

import os, sys
import subprocess
import re
import argparse

####################################################
# output example
#	                     FLASH_DSP0	IRAM  DRAM
#region size             1204	    128	  256
#usage	                 1000	    100	  200
#free 	                 204	    28	  56

DEBUG = False
collected_sections = {}
sections_footprint = {}
memory_configuration = {}

def parse_cfgfile(cfg_file):
    f = open(cfg_file, "r")
    index = 0
    while True:
        l = f.readline()
        l = l.strip()
        if not l:
            break
        if re.match("^#",l):
            continue
        if re.search("=>",l):
            key,value = l.split('=>')
            base,length,end = value.split(',')
            key = key.strip()
            base = base.strip()
            length = length.strip()
            end = end.strip()
            if(key and base and length and end):
                collected_sections[key] = [index, base, length, end]
                index += 1

    if DEBUG:
        for key,value in collected_sections.items():
            print("cfg:%s => %s" %(key,value))
        print("\n")

def collect_sections_from_mapfile(mapfile):
    memory_config_flag = 0
    symbol_begin_flag = 0
    count_section_flag = 0

    f = open(mapfile)
    while True:
        line = f.readline()

        if not line or re.match('START GROUP', line, re.I) or (re.match('^OUTPUT', line, re.I)) or (count_section_flag == len(collected_sections.keys())):
            break
        if re.match('^\s*$', line, re.I):
            continue
        if re.match('Memory Configuration', line, re.I):
            memory_config_flag = 1
            continue
        if re.match('Linker script and memory map', line, re.I):
            memory_config_flag = 0
            symbol_begin_flag = 1
            continue

        if(memory_config_flag == 1):
            #print 'find %s' %(line)
            name,origin,length = line.split()[0:3]
            memory_configuration[name] = [origin, length]
            if name in collected_sections:
                index = collected_sections[name][0]
                sections_footprint.setdefault(index, []).append(name)
                sections_footprint[index].append(int(length,16))
                if DEBUG:
                    print ('find memory configuration= %s' %(sections_footprint[index]))

        if(symbol_begin_flag == 1):
            for region,cfg in collected_sections.items():
                base_pattern = cfg[1]
                len_pattern = cfg[2]
                end_pattern = cfg[3]
                #print("#%s#" %(pattern))
                check_pattern = '^\s*0x[0-9a-f]+\s+'+end_pattern
                if re.match(check_pattern, line, re.I):
                    if DEBUG:
                        print("####find pattern %s: %s" %(end_pattern, line))
                    addr = line.split()[0]
                    region_info = []

                    if DEBUG:
                        print('find %s end addr= %s, region config= %s' %(region, addr, cfg))

                    if (base_pattern == 'ORIGIN') and (len_pattern == 'LENGTH'):
                        if region not in memory_configuration:
                            sys.stdout.error("not find ORIGIN for region %s" %(region))
                            continue
                        region_info = memory_configuration[region]
                    else:
                        region_info = collected_sections[region][1:3]

                    if DEBUG:
                        print("%s info=%s" %(region,region_info))

                    usage = (int(addr, 16) & (~0x10000000))  - int(region_info[0], 16)
                    free = int(region_info[1], 16) - usage
                    index = collected_sections[region][0]
                    sections_footprint[index].append(usage)
                    sections_footprint[index].append(free)
                    count_section_flag += 1

                    if DEBUG:
                        print('####region %s usage= %s, free= %s' %(region, usage, free))

                    break
    f.close()

def calculate_max_len():
    max_len = 0
    indexes = sorted(sections_footprint.keys())
    for index in indexes:
        values = sections_footprint[index]
        for value in values:
            if(max_len < len(str(value))):
                max_len = len(str(value))
    return max_len

def output_space(handle, space_count):
    i = 0
    while i<space_count:
        handle.write("%s" %(' '))
        i+=1

def output_stats(handle):
    indexes = sorted(sections_footprint.keys())
    max_len = calculate_max_len()

    #output region name
    handle.write("                ")
    for index in indexes:
        region = sections_footprint[index][0]
        handle.write("%s" %(region))
        space_count = max_len - len(region) + 4
        output_space(handle, space_count)
    handle.write("\n")

    #output region total size
    handle.write("region size     ")
    for index in indexes:
        length = sections_footprint[index][1]
        handle.write("%s" %(length))
        space_count = max_len - len(str(length)) + 4
        output_space(handle,space_count)
    handle.write("\n")

    #output region usage size
    handle.write("usage           ")
    for index in indexes:
        usage = sections_footprint[index][2]
        handle.write("%s" %(usage))
        space_count = max_len - len(str(usage)) + 4
        output_space(handle,space_count)
    handle.write("\n")

    #output region free size
    handle.write("free            ")
    for index in indexes:
        free = sections_footprint[index][3]
        handle.write("%s" %(free))
        space_count = max_len - len(str(free)) + 4
        output_space(handle,space_count)
    handle.write("\n")

def print_stats(output_file):
    #output to std
    output_stats(sys.stdout)

    #output to file
    f = open(output_file, "w")
    output_stats(f)
    f.close()

def parse_argument():
    parser = argparse.ArgumentParser(description='Airoha footprint generator tool.\n  usage:\n  footprint_gen.py <cfgfile> <map file> <output file>', formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('cfgfile', metavar='cfgfile', help='cfgfile is under the path ./mcu/tools/scripts/build/Footprint_gen/footprint_gen_{core}.cfg')
    parser.add_argument('mapfile', metavar='mapfile', help='mapfile is generated in build flow, and is located under the path out/AB{chip}/{projet}/AB{chip}.map')
    parser.add_argument('output_file', metavar='output_file', help='output the footprint info to this file')

    settings = parser.parse_args()
    try:
        f = open(settings.cfgfile,'r')
        f.close()
    except OSError as reason:
        print('cfg file error:'+str(reason))
        sys.exit(0)
    try:
        f = open(settings.mapfile,'r')
        f.close()
    except OSError as reason:
        print('map file error:'+str(reason))
        sys.exit(0)
    try:
        f = open(settings.output_file,'w')
        f.close()
    except OSError as reason:
        print('output file error:'+str(reason))
        sys.exit(0)

    return settings.cfgfile, settings.mapfile, settings.output_file, settings

if __name__ == '__main__':

    #1. get and check  the input parameters
    cfg_file, mapfile, output_file, settings = parse_argument()
    if DEBUG:
        print('mapfile path: %s' %(cfg_file))
        print('mapfile path: %s' %(mapfile))
        print('outputfile path: %s\n' %(output_file))

    #2. parse the cfg file to get the collect region info
    parse_cfgfile(cfg_file)

    #3. parse the map file
    collect_sections_from_mapfile(mapfile)

    #4. output footprint info
    print_stats(output_file)

    sys.exit(0)

