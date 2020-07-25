#include<regex.h>
#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>
#include"data_fields.h"

void Set_Status(int state);
void getNewName(char *OldName, char OldExt[10], char NewExt[10], char *NewName);
char * copyFiles(char *fileName, char *oldFileName);

void Process_Rename()
{
	DIR *d;
	regex_t reg;
	char *newFileName, *oldFileName, *regexp, *newFileFullName;
	int ret;
	struct dirent *dir;
	regexp = (char *)malloc(50 * sizeof(char));
	snprintf(regexp, 50 * sizeof(char), "%s%s%s", "(.)*.", SrcExt, "$");
	ret = regcomp(&reg, regexp, REG_ICASE|REG_EXTENDED);
	if(ret)
	{
		ErrorDesc = "Regular expression compilation error";
		Set_Status(0);
	}
	d = opendir(SrcPath);
	if(d == NULL)
	{
		printf("Error!\n");
		strcpy(ErrorDesc, "Directory not found");
		Set_Status(0);
	}
	while((dir = readdir(d)) != NULL)
	{
		ret = regexec(&reg, dir->d_name, 0, NULL, 0);
		if(!ret)
		{
			printf("Renaming %s...\n", dir->d_name);
			newFileName = (char *)malloc(300 * sizeof(char));		
			getNewName(dir->d_name, SrcExt, DestExt, newFileName);
			oldFileName = (char *)malloc(300 * sizeof(char));
			newFileFullName = (char *)malloc(300 * sizeof(char));
			snprintf(oldFileName, 300 * sizeof(char), "%s%s%s", SrcPath, "/", dir->d_name);
			snprintf(newFileFullName, 300 * sizeof(char), "%s%s%s", DestPath, "/", newFileName);
			if(SrcPath != DestPath)
				oldFileName = copyFiles(dir->d_name, oldFileName);
			printf("Old File Name:%s\nNew File Name:%s\n", oldFileName, newFileFullName);
			rename(oldFileName, newFileFullName);
			printf("Rename done\n");
			StatusDesc = (char *)malloc(1024 * sizeof(char));
			snprintf(StatusDesc, 1024 * sizeof(char), "%s%s%s%s%s", "Renamed from ", oldFileName, " to ", newFileName, "\0");
			printf("Updating status\n");
			Set_Status_Desc(StatusDesc);
			printf("Renamed from %s to %s\n", oldFileName, newFileName);

		}
	}
	Set_Status(2);
}

void getNewName(char *OldName, char OldExt[10], char NewExt[10], char *NewName)
{
	char *result;
	char OldNameTemp[100], OldExtTemp[10];
	int index, i;
	strcpy(OldNameTemp, OldName);
	strcpy(OldExtTemp, OldExt);
	strcat(OldNameTemp, "$");
	snprintf(OldExtTemp, 10 * sizeof(char), "%s%s%s", ".", OldExt, "$");
	result = strstr(OldNameTemp, OldExtTemp);
	index = result - OldNameTemp;
	for(i = 0; i <= index; i++)
		NewName[i] = OldNameTemp[i];
	NewName[i] = '\0';
	strcat(NewName, NewExt);
	NewName[strlen(NewName)] = '\0';
}

char * copyFiles(char *fileName, char *oldFileName)
{
	char *newFileName, ch;
	FILE *newFP, *oldFP;
	newFileName = (char *)malloc(1024 * sizeof(char));		
	snprintf(newFileName, 1024 * sizeof(char), "%s%s%s", DestPath, "/", fileName);
	printf("copying files from Old filename:%s to New filename:%s\n", oldFileName, newFileName);
	oldFP = fopen(oldFileName, "r");
	newFP = fopen(newFileName, "w");
	while((ch = fgetc(oldFP)) != EOF)
		fputc(ch, newFP);
	fclose(newFP);
	fclose(oldFP);
	return newFileName;
}


