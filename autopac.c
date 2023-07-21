#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

void createTarball(char *packageName, char *version, char *release) {
    char cmd[200];
    char cmd2[200];
    char cmd3[200];
    char cmd4[200];
    snprintf(cmd, sizeof(cmd), "cd ~/rpmbuild && tar -czvf %s-%s-%s.tar.gz -C %s .", packageName, version, release, packageName);
    system(cmd);
    snprintf(cmd2, sizeof(cmd2), "cp ~/rpmbuild/%s/* ~/rpmbuild/SOURCES", packageName);
    system(cmd2);
    snprintf(cmd3, sizeof(cmd3), "cp ~/rpmbuild/%s/*.spec* ~/rpmbuild/SPECS", packageName);
    system(cmd3);
    snprintf(cmd4, sizeof(cmd4), "cp ~/rpmbuild/autopac/%s-%s-%s.tar.gz ~/rpmbuild/SOURCES", packageName, version, release);
    system(cmd4);
    printf("Tarball created: %s-%s-%s.tar.gz\n", packageName, version, release);
}

void renameTarball(char *packageName, char *version, char *release) {
    char cmd[200];
    snprintf(cmd, sizeof(cmd), "mv ~/rpmbuild/SOURCES/%s-%s-%s.tar.gz ~/rpmbuild/SOURCES/%s-%s.tar.gz", packageName, version, release, packageName, version);
    system(cmd);
    printf("Tarball created: %s-%s-%s.tar.gz\n", packageName, version, release);
}

void createSRPM(char *packageName, char *version, char *release) {
    char cmd[500];
    snprintf(cmd, sizeof(cmd), "rpmdev-setuptree && rpmbuild -bs ~/rpmbuild/SPECS/%s.spec", packageName, version, release, packageName);
    system(cmd);

    if (system(cmd) != 0) {
        renameTarball(packageName, version, release);
        system(cmd);

    }
    printf("SRPM created: ~/rpmbuild/SRPMS/%s-%s-%s.src.rpm\n", packageName, version, release);
}

void buildPackage(char *packageName, char *version, char *release, char *chroot,  char *arch) {
    char cmd[200] = "mkdir ~/rpmbuild/RPMS/x86_64 ~/rpmbuild/RPMS/aarch64" ;
    system(cmd);
    char cmd2[200];
    snprintf(cmd2, sizeof(cmd2), "mock -r %s  --resultdir ~/rpmbuild/RPMS/%s --rebuild ~/rpmbuild/SRPMS/%s-%s-%s*.src.rpm", chroot, arch, packageName, version, release);
    system(cmd2);
    printf("Package build using mock.\n");
}

void selectChroot(char *chroot, char *arch) {
    char distro[50];
    char version[50];

    printf("\nSelect Distribution:\n");
    printf("1. Fedora\n");
    printf("2. CentOS-Stream\n");
    printf("3. CentOS-Stream+epel\n");
    printf("4. CentOS-Stream+epel-next\n");
    printf("5. Exit\n");
    printf("Select your distro: ");
    int distro_selection;
    scanf("%d", &distro_selection);

    switch (distro_selection) {
        case 1:
            strcpy(distro, "fedora");
            break;
        case 2:
            strcpy(distro, "centos-stream");
            break;
        case 3:
            strcpy(distro, "centos-stream+epel");
            break;
        case 4:
            strcpy(distro, "centos-stream+epel-next");
            break;
        case 5:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid selection! Please try again..\n");
            return;
    }

    int version_selection;
    char *dist = "centos";
    if (strstr(distro, dist) != NULL) {
        printf("1. 8\n");
        printf("2. 9\n");
        printf("Select your distro version: ");   
    }
    else {
        printf("1: 37\n");
        printf("2: 38\n");
        printf("3: Rawhide\n");
    }

    scanf("%d", &version_selection);
    if (strstr(distro, dist) != NULL) {
        switch (version_selection) {
            case 1:
                strcpy(version, "8");
                break;
            case 2:
                strcpy(version, "9");
                break;
            default:
                printf("Invalid selection! Please try again..\n");
                return;
        }
    }
    else {
        switch (version_selection) {
        case 1:
            strcpy(version, "37");
            break;
        case 2:
            strcpy(version, "38");
            break;
        case 3:
            strcpy(version, "rawhide");
            break;
        default:
            printf("Invalid selection! Please try again..\n");
            return;
        }
    }

    printf("\nSelect Architecture:\n");
    printf("1: x86_64\n");
    printf("2: aarch64\n");
    printf("3: Exit\n");
    printf("Select your architecture: ");
    int arch_selection;
    scanf("%d", &arch_selection);

    switch (arch_selection) {
        case 1:
            strcpy(arch, "x86_64");
            break;
        case 2:
            strcpy(arch, "aarch64");
            break;
        case 3:
            printf("Exiting...\n");
            exit(0);
        default:
           printf("Invalid selection! Please try again..\n");
        return;
    }

snprintf(chroot, 200, "%s-%s-%s", distro, version, arch);
printf("Chroot selected: %s\n", chroot);
}

void printHelp(){
    printf("Usage:.\n");
    printf("./auto_pac -n my-package -v 0.1 -r 1\n\n");
}

void runLint(char *packageName) {
    char cmd[200];
    snprintf(cmd, sizeof(cmd), "rpmlint -i ~/rpmbuild/%s", packageName);
    system(cmd);
    printf("Rpmlint complete for: %s.\n", packageName);
}

int main( int argc, char *argv[] ) {
char packageName [50];
char version [50];
char release [50];
char chroot[200];
char arch[20];
int menuOpt;
int option;

if (argc > 1) {
    while ((option = getopt(argc, argv, "hn:v:r:")) != -1) {
        switch (option) {
        case 'h':
            printHelp();
            return 0;
        case 'n':
            strcpy(packageName, optarg);
            break;
        case 'v':
            strcpy(version, optarg);
            break;
        case 'r':
            strcpy(release, optarg);
            break;
        default:
            printHelp();
            break;
        }
    }
}
else {
    printf("Package Name: ");
    scanf("%s", packageName);
    printf("Version: ");
    scanf("%s", version);
    printf("Release: ");
    scanf("%s", release);
}

while (1) {

    printf("\nMenu\n");
    printf("1. Create tarball\n");
    printf("2. Create SRPM\n");
    printf("3. Build package using mock\n");
    printf("4. Run rpmlint on %s\n", packageName);
    printf("5. Exit\n");

    printf("\nSelect option: ");
    scanf("%d", &menuOpt);

    switch (menuOpt) {
        case 1:
            createTarball(packageName, version, release);
            break;
        case 2:
            createSRPM(packageName, version, release);
            break;
        case 3:
            selectChroot(chroot, arch);
            buildPackage(packageName, version, release, chroot, arch);
            break;
        case 4:
            runLint(packageName);
            break;
        case 5:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid selection! Please try again..\n");
    }
}

return 0;
}
