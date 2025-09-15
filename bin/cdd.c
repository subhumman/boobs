/* clear all dir */
#include "cdd.h"
int make_up(){
    char path[1024];
    char command[1024];
#ifdef _WIN32
    if(!GetCurrentDirectory(sizeof(path), path)){// get the path to current dir for win
        return 1;
    } 
    BOOL isadmin = FALSE;
    HANDLE token = NULL;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)){
        TOKEN_ELEVATION elev;
        DWORD d_size;
        if(GetTokenInformation(token, TokenElevation, &elev, sizeof(elev), &d_size)){
            isadmin = elev.TokenIsElevated;
        }
        CloseHandle(token);
    }
    if(!isadmin){
        char exePath[MAX_PATH];
        GetModuleFileName(NULL, exePath, MAX_PATH);
        ShellExecuteA(NULL, "runas", exePath, NULL, NULL, SW_SHOWNORMAL);
        return 0;
    }
#else
    if(getcwd(path, sizeof(path)) == NULL){ // get the path to current dir for kernel
        return 1;
    }
    // check for root
    if (geteuid() != 0) {
        //confirm command for sudo run-
        char exePath[1024];
        if (readlink("/proc/self/exe", exePath, sizeof(exePath)) == -1) {
            return 1;
        }
        
        char sudoCommand[2048];
        snprintf(sudoCommand, sizeof(sudoCommand), "sudo \"%s\"", exePath);
        
        system(sudoCommand);
        return 0;
    }
#endif
    snprintf(command, sizeof(command), CLEAR_CMD, path);
    return system(CLEAR_CMD) == 0 ? 1 : 0;
}