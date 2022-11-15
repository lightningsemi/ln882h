
from ntpath import join
import os
import sys
import time
import binascii
import struct
import math

def str_to_bytes(s):
    return s.encode()

def bytes_to_str(b):
    return

def char_to_bytes(c):
    c = c & 0xFF
    s = hex(c).lstrip('0x');
    if len(s) < 2:
        for i in range(2 - len(s)):
            s = "0" + s 
    return s.upper().encode()

def int_to_bytes(i):
    i = i & 0xFFFFFFFF
    s = hex(i).lstrip('0x');
    if len(s) < 4:
        for i in range(4 - len(s)):
            s = "0" + s 
    return s.upper().encode()


def bin_to_hex(bin_file_path , hex_file_path , start_addr) -> bool:
    
    bin_file = open(bin_file_path, 'rb')
    hex_file = open(hex_file_path, 'wb+')

    bin_len  = os.path.getsize(bin_file_path)

    cur_base = start_addr >> 16
    cur_offset = start_addr & 0xFFFF

    highc = cur_base >> 8;
    lowc  = cur_base & 0xFF;
    cksum = (2 + 4 + highc + lowc) & 0xFF;
    cksum = 0xFF - cksum;
    cksum = cksum + 1;

    
    hex_file.write(str_to_bytes(':02000004'))
    hex_file.write(int_to_bytes(cur_base))
    hex_file.write(char_to_bytes(cksum))

    hex_file.write(struct.pack('B',0x0d))
    hex_file.write(struct.pack('B',0x0a))

    # for num in range(0,32):
    #buffer = str_to_bytes((bin_file.read(32).decode()))
    #buffer = buffer.replace('\x','')

    while bin_len > 32:
        if cur_offset <= 0xFFFF:

            hex_file.write(str_to_bytes(':20'))
            hex_file.write(int_to_bytes(cur_offset))
            hex_file.write(str_to_bytes('00'))

            highc = cur_offset >> 8;
            lowc  = cur_offset & 0xFF;
            cksum = (0x20 + highc + lowc) & 0xFF;

            buffer = bin_file.read(32)
            for num in range(0,32):
                hex_file.write(char_to_bytes(buffer[num]))
                cksum = (cksum + buffer[num]) & 0xFF

            cksum = 0xFF - cksum;
            cksum = cksum + 1;

            hex_file.write(char_to_bytes(cksum))
            hex_file.write(struct.pack('B',0x0d))
            hex_file.write(struct.pack('B',0x0a))

            cur_offset += 32;
            bin_len    -= 32;

        else:
            start_addr = start_addr + cur_offset;
            cur_base = start_addr >> 16
            cur_offset = start_addr & 0xFFFF
            highc = cur_base >> 8;
            lowc  = cur_base & 0xFF;
            cksum = (2 + 4 + highc + lowc) & 0xFF;
            cksum = 0xFF - cksum;
            cksum = cksum + 1;

            hex_file.write(str_to_bytes(':02000004'))
            hex_file.write(int_to_bytes(cur_base))
            hex_file.write(char_to_bytes(cksum))

            hex_file.write(struct.pack('B',0x0d))
            hex_file.write(struct.pack('B',0x0a))

            cur_offset = 0;

    if bin_len > 0:

        bin_len = bin_len & 0xFF

        hex_file.write(str_to_bytes(':'))
        hex_file.write(char_to_bytes(bin_len))
        hex_file.write(int_to_bytes(cur_offset))
        hex_file.write(str_to_bytes('00'))

        highc = cur_offset >> 8;
        lowc  = cur_offset & 0xFF;
        cksum = (bin_len + highc + lowc) & 0xFF;

        buffer = bin_file.read(bin_len)
        for num in range(0,bin_len):
            hex_file.write(char_to_bytes(buffer[num]))
            cksum = (cksum + buffer[num]) & 0xFF

        cksum = 0xFF - cksum;
        cksum = cksum + 1;

        hex_file.write(char_to_bytes(cksum))
        hex_file.write(struct.pack('B',0x0d))
        hex_file.write(struct.pack('B',0x0a))
    

    hex_file.write(str_to_bytes(':00000001FF'))
    hex_file.write(struct.pack('B',0x0d))
    hex_file.write(struct.pack('B',0x0a))

    hex_file.close()
    bin_file.close()
    
    bin_len  = os.path.getsize(bin_file_path)
    hex_len  = os.path.getsize(hex_file_path)
    except_len = bin_len * 2 + math.ceil(bin_len * 1.0 / 65536) * 17  + math.ceil(bin_len * 1.0 / 32) * 13 + 13
    
    if except_len == hex_len:
        return 1
    else:
        return 0
    

if __name__ == '__main__':
    ret = bin_to_hex('my_bin.bin','my_hex.hex',0x10000000)

    print("Generate hex file")
    if ret == 1 :
        now_time = time.localtime()
        time_string = "{yyyy}-{MM:02d}-{DD:02d} {hh:02d}:{mm:02d}:{ss:02d}".format(yyyy=now_time.tm_year,
        MM=now_time.tm_mon, DD=now_time.tm_mday,
        hh=now_time.tm_hour, mm=now_time.tm_min, ss=now_time.tm_sec)
        print("----------" * 6)
        print("\t\t\tbin done @ {t}".format(t=time_string))
        print("----------" * 6)
    else:
        print("Generate Error!!!")
