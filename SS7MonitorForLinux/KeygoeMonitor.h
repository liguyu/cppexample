#ifndef _KEYGOEMONITOR_H
#define _KEYGOEMONITOR_H

int makeargv(const char *s, const char * delimiters, char ***argvp);
long find(list *first, DeviceID_t dev, list *local);
void freemakeargv(char **argv);
//int mantainProc(const char const **chargvp, ACSHandle_t acsHandle, int numtokens);
//int monitorProc(const char const **chargvp, ACSHandle_t acsHandle, int numtokens);
int maintainProc(char **chargvp, ACSHandle_t acsHandle, int numtokens);
int monitorProc(char **chargvp, ACSHandle_t acsHandle, int numtokens, int row, int col);
int circuitReset(char **argv, char **);
int circuitBlocking(char **argv, char **);
int circuitUnblock(char **argv, char **);

#endif
