#ifndef FILE_H
#define FILE_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"

void list_dir(fs::FS &fs, const char *dirname, uint8_t levels);
void create_dir(fs::FS &fs, const char *path);
void remove_dir(fs::FS &fs, const char *path);
void read_file(fs::FS &fs, const char *path);
void write_file(fs::FS &fs, const char *path, const char *message);
void append_file(fs::FS &fs, const char *path, const char *message);
void rename_file(fs::FS &fs, const char *path1, const char *path2);
void delete_file(fs::FS &fs, const char *path);
void test_file_io(fs::FS &fs, const char *path);
void init_sd_card_reader();

#endif // FILE_H
