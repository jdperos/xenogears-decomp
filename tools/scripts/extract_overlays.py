import struct
import os
import math

# CDROM-XA Parser
# Author: https://github.com/ladysilverberg
class VolumeDescriptor:
    def __init__(self, data):
        self.vol_descriptor_type = data[0]
        self.standard_identifier = data[1:6]
        self.vol_descriptor_version = data[6:7]
        self.system_identifier = data[8:40]
        self.volume_identifier = data[0x28:0x48]
        self.vol_space_size = struct.unpack("<I", data[0x50:0x54])[0]
        self.vol_space_size_c = struct.unpack(">I", data[0x54:0x58])[0]
        assert self.vol_space_size == self.vol_space_size_c
        self.vol_seq_number = struct.unpack("<H", data[0x7C:0x7E])[0]
        vol_seq_number_c = struct.unpack(">H", data[0x7E:0x80])[0]
        assert self.vol_seq_number == vol_seq_number_c
        self.logical_block_size = struct.unpack("<H", data[0x80:0x82])[0]
        logical_block_size_c = struct.unpack(">H", data[0x82:0x84])[0]
        assert self.logical_block_size == logical_block_size_c
        self.pt_size = struct.unpack("<I", data[0x84:0x88])[0]
        pt_size_c = struct.unpack(">I", data[0x88:0x8C])[0]
        assert self.pt_size == pt_size_c
        self.pt1_block_number = struct.unpack("<I", data[0x8C:0x90])[0]
        self.pt2_block_number = struct.unpack("<I", data[0x90:0x94])[0]
        self.pt3_block_number = struct.unpack(">I", data[0x94:0x98])[0]
        self.pt4_block_number = struct.unpack(">I", data[0x98:0x9C])[0]
        self.app_identifier = data[0x23E:0x23E+128]
        self.copyright_filename = data[0x2BE:0x2BE+37]
        self.abstract_filename = data[0x2E3:0x2E3+37]
        self.bibliographic_filename = data[0x308:0x308+37]
        self.vol_creation_timestamp = data[0x32D:0x32D+17]
        self.cd_xa_id_sig = data[0x400:0x408]
        self.root_dir_record = DirectoryRecord(data[0x9C:0x9C+34])

    def print(self):
        print()
        print("Volume Descriptor")
        print("------------------")
        if self.vol_descriptor_type == 0x1:
            print("Volume Descriptor Type: Primary Volume Descriptor")
        elif self.vol_descriptor_type == 0x2:
            print("Volume Descriptor Type: Supplementary Volume Descriptor")
        print("Standard Identifier:", self.standard_identifier)
        if self.vol_descriptor_version == 0x1:
            print("Volume Descriptor Version: Standard")
        print("System Identifier:", self.system_identifier)
        print("Volume Identifier:", self.volume_identifier)
        print("Volume Sequence Number:", hex(self.vol_seq_number))
        print("Volume Space Size:", hex(self.vol_space_size))
        print("Logical block size:", hex(self.logical_block_size))
        print("Path Table Size in Bytes:", hex(self.pt_size))
        print("Path Table 1 Block Number:", self.pt1_block_number)
        print("Path Table 2 Block Number:", self.pt2_block_number)
        print("Path Table 3 Block Number:", self.pt3_block_number)
        print("Path Table 4 Block Number:", self.pt4_block_number)
        self.root_dir_record.print()
        print("Application Identifier:", self.app_identifier)
        print("Copyright Filename:", self.copyright_filename)
        print("Abstract Filename:", self.abstract_filename)
        print("Bibliographic Filename:", self.bibliographic_filename)
        print("Volume Creation Timestamp", self.vol_creation_timestamp)
        print("CD-XA Identifying Signature:", self.cd_xa_id_sig)
        print()

        #assert data[0x0] == 0x1 # Volume descriptor type == Primary volume descriptor
        #assert data[0x1 : 0x1 + 5] == b'CD001' # Standard Identifier
        #assert data[0x6 : 0x6 + 1] == b'\x01' # Volume descriptor version == Standard
        #assert data[0x8 : 0x8 + 11] == b'PLAYSTATION' # System Identifier

class DirectoryRecord:
    def __init__(self, data):
        self.length_of_directory_record = data[0]
        self.data_logical_block_num = struct.unpack("<I", data[2:6])[0]
        data_logical_block_num_c = struct.unpack(">I", data[6:10])[0]
        assert self.data_logical_block_num == data_logical_block_num_c
        self.data_size = struct.unpack("<I", data[10:14])[0]
        data_size_c = struct.unpack(">I", data[14:18])[0]
        assert self.data_size == data_size_c
        self.file_flags = data[0x19]
        self.name_len = data[0x20]
        self.name = data[0x21 : 0x21 + self.name_len]
    
    def print(self):
        print()
        print("Directory record")
        print("------------------")
        print("Length of Directory Record:", hex(self.length_of_directory_record))
        print("Data Logical Block Number:", self.data_logical_block_num)
        print("Data Size in Bytes:", hex(self.data_size))
        if self.file_flags == 0x0:
            print("File Flags: File")
        elif self.file_flags == 0x2:
            print("File Flags: Directory")
        print("Name:", self.name)
        print()

class PathTableEntry:
    def __init__(self, data):
        self.dir_name_len = data[0]
        self.ext_attr_record_len = data[1]
        self.dir_logical_block_num = data[2:6]
        self.parent_dir_number = data[6:8]
        self.dir_name = data[8:8+self.dir_name_len]
        self.size = self.dir_name_len + 8 + 1
    
    def print(self):
        print()
        print("Path Table Entry")
        print("------------------")
        print("Directory name:", self.dir_name)
        print()

class PathTable:
    def __init__(self, data, table_size):
        self.entries = []
        cur_idx = 0
        while cur_idx < table_size:
            entry = PathTableEntry(data[cur_idx : table_size])
            self.entries.append(entry)
            cur_idx += entry.size
    
    def print(self):
        for entry in self.entries:
            entry.print()

class CDROM_XA:
    def __init__(self, filename):
        self.handle = open(filename, "rb")
        self.sector_size = 2352
        self.sector_data_size = 0x800
        self.cur_sector = 0

    def __set_sector(self, sector):
        self.cur_sector = sector
        self.handle.seek(sector * self.sector_size, 0)

    def read_sector(self, sector):
        self.__set_sector(sector)
        return self.read_next_sector()

    def read_next_sector(self):
        sector_data = self.__read_sector_xa_mode2_form1()
        self.cur_sector += 1
        return sector_data

    def __read_sector_xa_mode2_form1(self):
        sync_pattern = self.handle.read(12)
        address = self.handle.read(3) # Minute, second, fraction/sector
        mode = self.handle.read(1)
        subheader = self.handle.read(4)
        copy_of_subheader = self.handle.read(4)
        data = self.handle.read(2048)
        error_detection = self.handle.read(4)
        error_correction = self.handle.read(276)
        assert mode == b'\x02'
        assert sync_pattern == b'\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00'
        assert subheader == copy_of_subheader
        return data
    
    def read_and_write_file_from_sector(self, sector, size, filename):
        f = open(filename, "wb")
        self.__set_sector(sector)
        n_sectors = math.ceil(size / 0x800)
        for i in range(n_sectors):
            f.write(self.read_next_sector())
        f.close()

    def __unpack_directory(self, records, cur_path):
        for record in records:
            print("Current Record")
            record.print()

            # Self and parents entries
            if record.name == b'\x00' or record.name == b'\x01':
                continue

            # Directory
            if record.file_flags == 0x2:
                # Create new directory if it doesn't exist
                dir_path = cur_path + "/" + record.name.decode()
                if not os.path.exists(dir_path): 
                    os.makedirs(dir_path) 
                
                # Read records in new directory and recursively unpack it
                dir_records_sector = self.read_sector(record.data_logical_block_num)
                dir_records = self.__parse_directory_records(dir_records_sector)
                self.__unpack_directory(dir_records, dir_path)

            # File
            if record.file_flags == 0x0:
                # Read filename
                filename = record.name.decode().split(";")[0]

                # Read file contents
                self.__set_sector(record.data_logical_block_num)
                bytes_read = 0x0

                file_handle = open(cur_path + "/" + filename, "wb")

                while bytes_read < record.data_size:
                    sector_data = self.read_next_sector()
                    print("Progress...", hex(bytes_read), "/", hex(record.data_size))

                    # Last sector?
                    if bytes_read + self.sector_data_size >= record.data_size:
                        append_size = record.data_size - bytes_read
                        file_handle.write(sector_data[0 : append_size])
                        bytes_read += append_size
                    else:
                        file_handle.write(sector_data)
                        bytes_read += self.sector_data_size
                
                assert bytes_read == record.data_size
                file_handle.close()

    def __parse_directory_records(self, sector_data):
        records = []
        cur_index = 0
        while True:
            dir_record = DirectoryRecord(sector_data[cur_index::])
            if dir_record.length_of_directory_record <= 0:
                break
            cur_index += dir_record.length_of_directory_record
            records.append(dir_record)
        return records

# Disc decoding utility for Xenogears, since the game uses its own archive system
class XenogearsDecoder:
    def __init__(self, header, table):
        self.header = header
        self.table = table
        self.current_decoding_value = 0
        
    def unpack_overlays(self, target_dir, cdrom_xa):
        OVERLAYS = [
            ("FIELD.LZSS.BIN", 0xE),
            ("BATTLE.LZSS.BIN", 0x10),
            ("WORLD_MAP.LZSS.BIN", 0xF),
            ("BATTLING.LZSS.BIN", 0xD),
            ("MENU.LZSS.BIN", 0x11),
            ("MOVIE.LZSS.BIN", 0x12)
        ]

        # Overlays => 0x8004eaa0
        for i in range(len(OVERLAYS)):
            ovl = OVERLAYS[i]
            overlay_offset = ovl[1]
            self.set_decoding_value(0, 1)
            sector = self.get_sector(overlay_offset)
            size = self.get_size_aligned(overlay_offset)
            file_path = target_dir + "/" + ovl[0]
            cdrom_xa.read_and_write_file_from_sector(sector, size, file_path)
            print("Unpacked LZSS compressed overlay: ", file_path)
    
    def set_decoding_value(self, v1, v2):
        offset = (v1 + v2) * 2
        self.current_decoding_value = self.header[offset]
        self.current_decoding_value += (self.header[offset + 1] << 8)
        self.current_decoding_value -= 1
    
    def get_sector(self, index):
        offset = (index + self.current_decoding_value - 1) * 7
        sector = self.table[offset]
        sector += (self.table[offset + 1] * 0x100)
        sector += (self.table[offset + 2] * 0x10000)
        return sector
    
    def get_size(self, index):
        offset = (index + self.current_decoding_value - 1) * 7
        size = (self.table[offset + 6] * 0x1000000)
        size += (self.table[offset + 5] * 0x10000)
        size += (self.table[offset + 4] * 0x100)
        size += self.table[offset + 3]
        return size

    def get_size_aligned(self, index):
        size = self.get_size(index)
        v = size + 3
        if v < 0:
            v = size + 6
        return (v >> 2) << 2

def lzss_decompress(input_file, output_file):
    in_file = open(input_file, "rb")
    out_file = open(output_file, "wb+")
    comp_size = os.path.getsize(input_file)
    decomp_size = struct.unpack("<I", in_file.read(4))[0]

    while out_file.tell() < decomp_size:
        ctl_byte = int.from_bytes(in_file.read(1), "little")
        ctl_bit = ctl_byte & 1
        for i in range(8):
            if in_file.tell() >= comp_size:
                in_file.close()
                out_file.close()
                return
            
            ctl_byte = ctl_byte >> 1

            if ctl_bit == 0:
                out_file.write(in_file.read(1))
            else:
                v1 = int.from_bytes(in_file.read(1), "little")
                v0 = int.from_bytes(in_file.read(1), "little")
                offset = ((v0 & 0xF) << 8) | v1
                length = (v0 >> 4) + 3

                of_cur = out_file.tell()
                readback = out_file.tell() - offset

                for i in range(length):
                    out_file.seek(readback + i)
                    tmp = out_file.read(1)
                    out_file.seek(of_cur + i)
                    out_file.write(tmp)
            ctl_bit = ctl_byte & 1

    in_file.close()
    out_file.close()

# Overlay extraction
if not os.path.exists("./Xenogears.bin"):
    print("Please put your dumped Xenogears Disc 1 in the same directory as this script.")
    print("Also, make sure to name it Xenogears.bin")
    exit()

disc = CDROM_XA("Xenogears.bin")
header = disc.read_sector(0x28)
disc.cur_sector = 0x18
disc.handle.seek(0x18 * disc.sector_size, 0)
table = b''
for i in range(0x10):
    table += disc.read_next_sector()
decoder = XenogearsDecoder(header, table)
decoder.unpack_overlays(".", disc)
disc.handle.close()

# Decompress overlays
print("Decompressing...")
lzss_decompress("FIELD.LZSS.BIN", "FIELD.BIN")
lzss_decompress("BATTLE.LZSS.BIN", "BATTLE.BIN")
lzss_decompress("WORLD_MAP.LZSS.BIN", "WORLD_MAP.BIN")
lzss_decompress("BATTLING.LZSS.BIN", "BATTLING.BIN")
lzss_decompress("MENU.LZSS.BIN", "MENU.BIN")
lzss_decompress("MOVIE.LZSS.BIN", "MOVIE.BIN")

print("Extracted and decompressed all overlays")