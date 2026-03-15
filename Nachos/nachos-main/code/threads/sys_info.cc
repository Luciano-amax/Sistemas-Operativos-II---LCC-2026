#include "sys_info.hh"
// Modificación ejercicio 12: se agregan estos headers para usar constantes de memoria y disco.
#include "machine/disk.hh"
#include "machine/mmu.hh"
#include "threads/system.hh"
// Fin de la modificacion de estos headers del ejercicio 12 :]

#include "copyright.h"
#include "filesys/directory_entry.hh"
#include "filesys/file_system.hh"
#include "filesys/raw_file_header.hh"

#include <stdio.h>


void SysInfo()
{
    (void) COPYRIGHT;  // Prevent warning about unused variable.

    const char *OPTIONS =
#ifdef THREADS
      "THREADS "
#endif
#ifdef USER_PROGRAM
      "USER_PROGRAM "
#endif
#ifdef VMEM
      "VMEM "
#endif
#ifdef FILESYS
      "FILESYS "
#endif
#ifdef FILESYS_STUB
      "FILESYS_STUB "
#endif
#ifdef USE_TLB
      "USE_TLB "
#endif
#ifdef FILESYS_NEEDED
      "FILESYS_NEEDED "
#endif
#ifdef DFS_TICKS_FIX
      "DFS_TICKS_FIX "
#endif
      ;

    printf("System information.\n");
    printf("\n\
General:\n\
  Nachos release: %s %s.\n\
  Option definitions: %s\n",
      PROGRAM, VERSION, OPTIONS);
    // Modificación para el ejercicio 12 (PLANCHA 1): mostrar la capacidad real de memoria.
    unsigned pageSize = PAGE_SIZE;
    unsigned numPages = DEFAULT_NUM_PHYS_PAGES;
    unsigned tlbEntries = TLB_SIZE;
#ifdef USER_PROGRAM
    if (machine != nullptr) {
        numPages = machine->GetNumPhysicalPages();
    }
#endif
    unsigned memorySize = numPages * pageSize;
    printf("\n\
Memory:\n\
  Page size: %u bytes.\n\
  Number of pages: %u.\n\
  Number of TLB entries: %u.\n\
  Memory size: %u bytes.\n",
      pageSize, numPages, tlbEntries, memorySize);
    // Modificación para el ejercicio 12 (PLANCHA 1): mostrar los parámetros reales del disco.
    unsigned sectorSize = SECTOR_SIZE;
    unsigned sectorsPerTrack = SECTORS_PER_TRACK;
    unsigned numTracks = NUM_TRACKS;
    unsigned numSectors = NUM_SECTORS;
    unsigned diskSize = numSectors * sectorSize;
    printf("\n\
Disk:\n\
  Sector size: %u bytes.\n\
  Sectors per track: %u.\n\
  Number of tracks: %u.\n\
  Number of sectors: %u.\n\
  Disk size: %u bytes.\n",
      sectorSize, sectorsPerTrack, numTracks, numSectors, diskSize);
    printf("\n\
Filesystem:\n\
  Sectors per header: %u.\n\
  Maximum file size: %u bytes.\n\
  File name maximum length: %u.\n\
  Free sectors map size: %u bytes.\n\
  Maximum number of dir-entries: %u.\n\
  Directory file size: %u bytes.\n",
      NUM_DIRECT, MAX_FILE_SIZE, FILE_NAME_MAX_LEN,
      FREE_MAP_FILE_SIZE, NUM_DIR_ENTRIES, DIRECTORY_FILE_SIZE);
}
