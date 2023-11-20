#include<stdio.h>
#include<avr/io.h>
#include"ffconf.h"
#include "ff.h"
#include "diskio.h"

int main()
{

FRESULT f_err_code;
static FATFS FATFS_Obj;
 
disk_initialize(0);
f_err_code = f_mount(0, &FATFS_Obj);

FIL fil_obj;
 
int variableName = 5;
 
f_open(&fil_obj, "/moo/foo.txt", FA_WRITE);
 
fprintf(&fil_obj, "bar %d", variableName);
 
f_close(&fil_obj);

return 0;

}
