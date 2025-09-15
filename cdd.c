#include "cdd.h"

int make_up(int argc, char** argv) {
    char path[1024];
    char command[2048];
    
#ifdef _WIN32
    if(!GetCurrentDirectoryA(sizeof(path), path)) {
        return 1;
    } 
    
    BOOL isadmin = FALSE;
    HANDLE token = NULL;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        TOKEN_ELEVATION elev;
        DWORD d_size;
        if(GetTokenInformation(token, TokenElevation, &elev, sizeof(elev), &d_size)) {
            isadmin = elev.TokenIsElevated;
        }
        CloseHandle(token);
    }
    
    if(!isadmin) {
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);
        ShellExecuteA(NULL, "runas", exePath, NULL, NULL, SW_SHOWNORMAL);
        return 0;
    }
#else
    if(getcwd(path, sizeof(path)) == NULL) {
        return 1;
    }
    
    if (geteuid() != 0) {
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
    printf("Executing: %s\n", command);
    
    return system(command) == 0 ? 0 : 1;
}