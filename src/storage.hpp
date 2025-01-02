#ifndef FILE_H
#define FILE_H

#include <FS.h>
#include <SD.h>
#include <SPIFFS.h>
#include <SPI.h>
#include <esp_log.h>

void init_storage();
bool is_file_available(fs::FS &fs, const char *path);

#endif // FILE_H
