#include "lib_tar.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

static unsigned int calculate_checksum(tar_header_t*header){//helper
    unsigned int sum=0;
    char*p=(char *)header;
    
    for(int i=0;i<BLOCK_SIZE;i++){sum+=(unsigned char)p[i];} //calcul taille totale du header
    for(int i=0;i<8;i++){sum-=(unsigned char)header->chksum[i];} //soustrait la valeur actuelle du checksum et met des espaces a la place
    sum+=8*' '; 
    
    return sum;
}

static int is_null_header(tar_header_t*header){//helper
    char*p=(char*)header;
    for(int i=0;i<BLOCK_SIZE;i++){if(p[i]!=0)return 0;}
    return 1;
}

/**
 * Checks whether the archive is valid.
 *
 * Each non-null header of a valid archive has:
 *  - a magic value of "ustar" and a null,
 *  - a version value of "00" and no null,
 *  - a correct checksum
 *
 * @param tar_fd A file descriptor pointing to the start of a file supposed to contain a tar archive.
 *
 * @return a zero or positive value if the archive is valid, representing the number of non-null headers in the archive,
 *         -1 if the archive contains a header with an invalid magic value,
 *         -2 if the archive contains a header with an invalid version value,
 *         -3 if the archive contains a header with an invalid checksum value
 */
int check_archive(int tar_fd){
    tar_header_t h;
    int count=0;

    //lseek(tar_fd,0,SEEK_SET); revient au début du fichier, pas un pbs normalement
    
    while(read(tar_fd,&h,BLOCK_SIZE)==BLOCK_SIZE){

        if(is_null_header(&h)){break;}
        
        if(strncmp(h.magic,TMAGIC,TMAGLEN)!=0){return-1;}
        
        if(strncmp(h.version,TVERSION,TVERSLEN)!=0){return-2;}
        
        unsigned int expected=TAR_INT(h.chksum);
        unsigned int check=calculate_checksum(&h);
        if(expected!=check){return-3;}
    
        count++;
        
        size_t size=TAR_INT(h.size);
        size_t blocs=(size+BLOCK_SIZE-1)/BLOCK_SIZE;
        lseek(tar_fd,blocs*BLOCK_SIZE,SEEK_CUR); //on saute les donnés du fichier pour aller au header suivant
    }
    return count;
}

/* Helper: Find a header by path */
static int find_header(int tar_fd,char*path,tar_header_t*h){
    //lseek(tar_fd,0,SEEK_SET); pas nécéssaire car on commence toujours au début du fichier
    while(read(tar_fd,h,BLOCK_SIZE)==BLOCK_SIZE){
        if(is_null_header(h)){return 0;}
        
        if(strcmp(h->name,path)==0){return 1;}
        
        size_t file_size=TAR_INT(h->size);
        size_t blocs=(file_size+BLOCK_SIZE-1)/BLOCK_SIZE;
        lseek(tar_fd,blocs*BLOCK_SIZE,SEEK_CUR); //on saute les donnés du fichier pour aller au header suivant
    }
    return 0;
}

/**
 * Checks whether an entry exists in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive,
 *         any other value otherwise.
 */
int exists(int tar_fd,char*path){
    if(!path)return 0;
    tar_header_t h;
    return find_header(tar_fd,path,&h);
}

/**
 * Checks whether an entry exists in the archive and is a directory.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a directory,
 *         any other value otherwise.
 */
int is_dir(int tar_fd,char*path) {
    tar_header_t h;
    
    if(!find_header(tar_fd,path,&h)){return 0;} //ici on utilise pas existe car on a besoin d'initialiser le header
    return(h.typeflag==DIRTYPE);
}

/**
 * Checks whether an entry exists in the archive and is a file.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a file,
 *         any other value otherwise.
 */
int is_file(int tar_fd, char *path) {
   tar_header_t h;
    
    if(!find_header(tar_fd,path,&h)){return 0;}
    return(h.typeflag==REGTYPE||h.typeflag==AREGTYPE);
}

/**
 * Checks whether an entry exists in the archive and is a symlink.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 * @return zero if no entry at the given path exists in the archive or the entry is not symlink,
 *         any other value otherwise.
 */
int is_symlink(int tar_fd, char *path) {
    tar_header_t h;
    
    if(!find_header(tar_fd,path,&h)){return 0;}
    return(h.typeflag==SYMTYPE);
}

/**
 * Lists the entries at a given path in the archive.
 * list() does *not* recurse into the directories listed at the given path.
 * If the path is NULL, it lists the entries at the root of the archive.
 *
 * Example:
 *  dir/          list(..., "dir/", ...) lists "dir/a", "dir/b", "dir/c/" and "dir/e/"
 *   ├── a
 *   ├── b
 *   ├── c/
 *   │   └── d
 *   └── e/
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive. If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param entries An array of char arrays, each one is long enough to contain a tar entry path.
 * @param no_entries An in-out argument.
 *                   The caller set it to the number of entries in `entries`.
 *                   The callee set it to the number of entries listed.
 *
 * @return zero if no directory at the given path exists in the archive,
 *         1 in case of success,
 *         -1 in case of error.
 */
int list(int tar_fd, char *path, char **entries, size_t *no_entries) {
    if (!entries || !no_entries) return -1;

    size_t max = *no_entries;
    *no_entries = 0;

    tar_header_t h;
    char scan_path[101] = {0};
    char base_path[101] = {0};
    int resolved_symlink = 0;

    /* ---------- Validation du path ---------- */
    if (path != NULL) {
        lseek(tar_fd, 0, SEEK_SET);
        if (!find_header(tar_fd, path, &h))
            return 0;

        strncpy(base_path, path, 100);
        strncpy(scan_path, path, 100);

        if (h.typeflag == SYMTYPE) {
            strncpy(scan_path, h.linkname, 100);
            resolved_symlink = 1;

            lseek(tar_fd, 0, SEEK_SET);
            if (!find_header(tar_fd, scan_path, &h))
                return 0;
        }

        if (h.typeflag != DIRTYPE)
            return 0;
    }

    size_t scan_len = path ? strlen(scan_path) : 0;

    lseek(tar_fd, 0, SEEK_SET);

    /* ---------- Parcours de l’archive ---------- */
    while (read(tar_fd, &h, BLOCK_SIZE) == BLOCK_SIZE) {
        if (is_null_header(&h)) break;

        char name[101] = {0};
        strncpy(name, h.name, 100);

        int match = 0;
        const char *suffix = NULL;

        if (path == NULL) {
            char *slash = strchr(name, '/');
            match = (!slash || slash == name + strlen(name) - 1);
            suffix = name;
        } else if (strncmp(name, scan_path, scan_len) == 0 &&
                   strlen(name) > scan_len) {

            suffix = name + scan_len;
            if (*suffix == '/') suffix++;

            char *slash = strchr(suffix, '/');
            match = (!slash || slash == suffix + strlen(suffix) - 1);
        }

        if (match && *no_entries < max) {
            if (path == NULL || !resolved_symlink) {
                strcpy(entries[*no_entries], name);
            } else {
                snprintf(entries[*no_entries], 256,
                         "%s/%s", base_path, suffix);
            }
            (*no_entries)++;
        }

        size_t size = TAR_INT(h.size);
        size_t blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
        lseek(tar_fd, blocks * BLOCK_SIZE, SEEK_CUR);
    }

    return 1;
}




/**
 * Adds a file at the end of the archive, at the archive's root level.
 * The archive's metadata must be updated accordingly.
 * For the file header, only the name, size, typeflag, magic value (to "ustar"), version value (to "00") and checksum fields need to be correctly set.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param filename The name of the file to add. If an entry already exists with the same name, the file is not written, and the function returns -1.
 * @param src A source buffer containing the file content to add.
 * @param len The length of the source buffer.
 *
 * @return 0 if the file was added successfully,
 *         -1 if the archive already contains an entry at the given path,
 *         -2 if an error occurred
 */
int add_file(int tar_fd,char*filename,uint8_t*src,size_t len){
    if(!filename||!src){return -2;}

    lseek(tar_fd,0,SEEK_SET);
    if(exists(tar_fd,filename)){return -1;}
    
    tar_header_t h;
    /* Find the end of the archive (skip all headers and their data) */
    lseek(tar_fd,0,SEEK_SET);
    while(read(tar_fd,&h,BLOCK_SIZE)==BLOCK_SIZE){
        if(is_null_header(&h)){break;}
        size_t file_size=TAR_INT(h.size);
        size_t blocks=(file_size+BLOCK_SIZE-1)/BLOCK_SIZE;
        lseek(tar_fd,blocks*BLOCK_SIZE,SEEK_CUR);
    }

    tar_header_t new;
    memset(&new,0,BLOCK_SIZE);
    
    strncpy(new.name,filename,99);
    memcpy(new.magic,TMAGIC,TMAGLEN-1);
    memcpy(new.version,TVERSION,TVERSLEN);
    
    new.typeflag=REGTYPE;
    
    snprintf(new.size,12,"%011lo",(unsigned long)len);
    
    unsigned int checksum=calculate_checksum(&new);
    snprintf(new.chksum,8,"%06o",checksum);
    
    if(write(tar_fd,&new,BLOCK_SIZE)!=BLOCK_SIZE){return -2;}
    
    if(len>0){if(write(tar_fd,src,len)!=(ssize_t)len){return -2;}}
    
    size_t padding_needed=(BLOCK_SIZE-(len%BLOCK_SIZE))%BLOCK_SIZE;
    if(padding_needed>0){
        uint8_t padding[BLOCK_SIZE]={0};
        if(write(tar_fd,padding,padding_needed)!=(ssize_t)padding_needed){return -2;}
    }
    
    /* Write two null blocks at the end */
    uint8_t null_block[BLOCK_SIZE] = {0};
    if(write(tar_fd,null_block,BLOCK_SIZE)!=BLOCK_SIZE){return -2;}
    if(write(tar_fd,null_block,BLOCK_SIZE)!=BLOCK_SIZE){return -2;}
    
    return 0;
}