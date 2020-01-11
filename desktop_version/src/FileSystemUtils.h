#ifndef FILESYSTEMUTILS_H
#define FILESYSTEMUTILS_H

#include <string>
#include <vector>
#include "Game.h"

int FILESYSTEM_init(char *argvZero);
void FILESYSTEM_deinit();

char *FILESYSTEM_getUserSaveDirectory();
char *FILESYSTEM_getUserLevelDirectory();

void FILESYSTEM_loadFileToMemory(const char *name, unsigned char **mem, size_t *len);
void FILESYSTEM_freeMemory(unsigned char **mem);

growing_vector<std::string> FILESYSTEM_getLevelDirFileNames();

#endif /* FILESYSTEMUTILS_H */
