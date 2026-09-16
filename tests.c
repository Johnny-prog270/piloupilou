#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib_tar.h"

/**

*void debug(const uint8_t *bytes, size_t len) {
*    for (int i = 0; i < len;) {
*        printf("%04x:  ", (int) i);
*
*        for (int j = 0; j < 16 && i + j < len; j++) {
*            printf("%02x ", bytes[i + j]);
*        }
*        printf("\t");
*        for (int j = 0; j < 16 && i < len; j++, i++) {
*            printf("%c ", bytes[i]);
*        }
*        printf("\n");
*   }
*}
*/

int total_t=0;
int passed_t=0;

void assert(const char*test_name,long expected,long actual,int is_bool){
    total_t++;
    int ok;
    if(is_bool){
        ok=expected?(actual!=0):(actual==0);
        if(ok){
            printf("OK: %s\n",test_name);
            passed_t++;
        }else{
            printf("Erreur: %s\n",test_name);
        }
    }else{
        ok=(actual==expected);
        if(ok){
            printf("OK: %s\n",test_name);
            passed_t++;
        }else{
            printf("Erreur: %s (expected=%ld, got=%ld)\n",test_name,expected,actual);
        }
    }
}

void create_test_archive(const char*filename) {
    int fd=open(filename,O_CREAT|O_WRONLY|O_TRUNC,0644);
    if(fd==-1){
        perror("create_test_archive");
        return;
    }
    
    tar_header_t h;
    memset(&h,0,sizeof(tar_header_t));
    
    //"file1.txt"
    memcpy(h.name,"file1.txt",strlen("file1.txt"));
    memcpy(h.mode,"0000644",7);
    memcpy(h.uid,"0000000",7);
    memcpy(h.gid,"0000000",7);
    memcpy(h.size,"0000005",7);
    memcpy(h.mtime,"00000000000",11);
    memcpy(h.magic,TMAGIC,TMAGLEN-1);
    memcpy(h.version,TVERSION,TVERSLEN);
    h.typeflag=REGTYPE;
    
    //Calculate checksum
    unsigned int sum=0;
    char*p=(char*)&h;
    for(int i=0;i<512;i++){sum+=(unsigned char)p[i];}
    for(int i=0;i<8;i++){sum-=(unsigned char)h.chksum[i];}
    sum+=8*' ';
    snprintf(h.chksum,8,"%06o",sum);
    
    write(fd,&h,512);
    write(fd,"hello",5);
    
    //Padding
    uint8_t padding[512]={0};
    write(fd,padding,507);
    
    //"test/dir/"
    memset(&h,0,sizeof(tar_header_t));
    memcpy(h.name,"test/",strlen("test/"));
    memcpy(h.mode,"0000755",7);
    memcpy(h.size,"0000000",7);
    memcpy(h.magic,TMAGIC,TMAGLEN-1);
    memcpy(h.version,TVERSION,TVERSLEN);
    h.typeflag=DIRTYPE;
    
    sum=0;
    p=(char*)&h;
    for(int i=0;i<512;i++){sum+=(unsigned char)p[i];}
    for(int i=0;i<8;i++){sum-=(unsigned char)h.chksum[i];}
    sum+=8*' ';
    snprintf(h.chksum,8,"%06o",sum);
    
    write(fd,&h,512);

    write(fd,padding,512);
    write(fd,padding,512);
    
    close(fd);
}

void test_check_archive(){
    const char*test_file="test_archive.tar";
    create_test_archive(test_file);
    
    int fd=open(test_file, O_RDONLY);
    if(fd==-1){
        printf("test ouverture archive\n");
        return;
    }
    
    int result=check_archive(fd);
    assert("check correct archive", 1, (result >= 0), 1);
    close(fd);
    
    //bad magic
    int fd_bad=open("bad_magic.tar",O_CREAT|O_WRONLY|O_TRUNC,0644);
    tar_header_t h;
    memset(&h,0,sizeof(tar_header_t));
    memcpy(h.name,"badfile",strlen("badfile"));
    memcpy(h.magic,"XXXXX",5);
    memcpy(h.version,TVERSION,TVERSLEN);
    write(fd_bad,&h,512);
    close(fd_bad);
    
    fd_bad=open("bad_magic.tar",O_RDONLY);
    result=check_archive(fd_bad);
    assert("check incorrect archive",-1,result,0);
    close(fd_bad);
    
    unlink(test_file);
    unlink("bad_magic.tar");
}

void test_exists(){
    const char*test_file="test_exists.tar";
    create_test_archive(test_file);
    
    int fd=open(test_file,O_RDONLY);
    assert("exists file",1,exists(fd,"file1.txt"),1);
    
    assert("exists directory",1,exists(fd,"test/"),1);
    
    assert("exists file inexistant",0,exists(fd,"nonexistent.txt"),1);
    
    assert("exists NULL path",0,exists(fd, NULL),1);
    
    assert("exists empty string",0,exists(fd, ""),1);
    
    close(fd);
    unlink(test_file);
}

void test_is_dir(){
    const char*test_file="test_is_dir.tar";
    create_test_archive(test_file);
    
    int fd=open(test_file,O_RDONLY);
    
    assert("is_dir directory",1,is_dir(fd,"test/"),1);
    
    assert("is_dir file",0,is_dir(fd,"file1.txt"),1);
    
    assert("is_dir path inexistant",0,is_dir(fd,"nonexistent/"),1);
    
    close(fd);
    unlink(test_file);
}

void test_is_file(){
    const char*test_file="test_is_file.tar";
    create_test_archive(test_file);
    
    int fd=open(test_file,O_RDONLY);
    
    assert("is_file file",1,is_file(fd,"file1.txt"),1);
    
    assert("is_file directory",0,is_file(fd,"test/"),1);
    
    assert("is_file path inexistant",0,is_file(fd,"nonexistent.txt"),1);
    
    close(fd);
    unlink(test_file);
}

void test_is_symlink(){
    const char*test_file="test_is_symlink.tar";
    create_test_archive(test_file);
    
    int fd=open(test_file, O_RDONLY);
    
    assert("is_symlink()file",0,is_symlink(fd,"file1.txt"),1);
    
    assert("is_symlink() directory",0,is_symlink(fd,"test/"),1);
    
    assert("is_symlink() path inexistant",0,is_symlink(fd,"link.lnk"),1);
    
    close(fd);
    unlink(test_file);
}

void test_list(){
    const char*test_file="test_list.tar";
    create_test_archive(test_file);
    
    int fd=open(test_file,O_RDONLY);
    
    char entries_buf[10][256];
    char*entries[10];
    for(int i=0;i<10;i++){entries[i]=entries_buf[i];}
    
    size_t no_entries=10;
    int result=list(fd,NULL,entries,&no_entries);
    //printf("list(NULL) returned %d with %zu entries\n",result,no_entries);
    assert("list root",1,result,0);
    
    no_entries=10;
    result=list(fd,"test/",entries,&no_entries);
    //printf("list(test/) returned %d with %zu entries\n",result,no_entries);
    
    no_entries=10;
    result=list(fd,"nonexistent/",entries,&no_entries);
    assert("list directory inexistant",0,result,0);
    
    no_entries=10;
    result=list(fd,"file1.txt",entries,&no_entries);
    assert("list file",0,result,0);
    
    close(fd);
    unlink(test_file);
}

void test_add_file(){
    const char*test_file="test_add.tar";
    create_test_archive(test_file);
    
    int fd=open(test_file,O_RDWR);
    
    uint8_t content[]="new file content";
    int result=add_file(fd,"newfile.txt",content,strlen((char*)content));
    assert("add_file correct",0,result,0);
    
    close(fd);
    fd=open(test_file,O_RDWR);
    
    result=add_file(fd,"file1.txt",content,strlen((char*)content));
    assert("add_file file déjà existant",-1,result,0);
    
    result=add_file(fd,NULL,content,strlen((char*)content));
    assert("add_file NULL filename",-2,result,0);
    
    result=add_file(fd,"anotherfile.txt",NULL,10);
    assert("add_file NULL buffer",-2,result,0);
    
    result=add_file(fd,"empty.txt",NULL,0);
    assert("add_file len=0",-2,result,0);
    
    uint8_t empty[]="";
    result=add_file(fd,"empty.txt",empty,0);
    assert("add_file empty content",0,result,0);
    
    close(fd);
    unlink(test_file);
}

int main(int argc,char**argv){
    //printf("Tests")
    
    test_check_archive();
    test_exists();
    test_is_dir();
    test_is_file();
    test_is_symlink();
    test_list();
    test_add_file();
    //printf("  Test Results: %d/%d passed\n", passed_tests, total_tests);
    
    return(passed_t==total_t)?0:1;
}