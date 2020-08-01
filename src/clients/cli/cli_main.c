//
// cli_main.c Proof of Concept CLI C Client
// TODO: Have this build on Windows
//
#include <stdlib.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "../../defs.h"

int ClientState;
bool ClientQuit = false;

/*
=====================
Client_Banner
=====================
Print a 'pretty' banner
*/
void Client_Banner() {
    printf("===========================================\n");
    printf(" C-CORD: THE \"\"BEST\"\" DISCORD CLIENT EVER.\n");
    printf("===========================================\n");
    printf("dont worry!! we use TLS for post requests!!\n");
    printf("and we don't print passwords either bc we\n");
    printf("are competent :^))\n");
    printf("-------------------------------------------\n");
}

/*
=====================
Client_Login
=====================
Ask For User Credentials then Login
*/
void Client_Login() {
    char* Email = malloc(sizeof(char)*64);
    char* Password = malloc(sizeof(char)*64);

    printf("Discord Email: ");
    scanf("%s", Email);
    
    // Hide password(!!!)
    // WIIIIIIIIIINDOOOOOOOOOOOOOOWS
    #ifndef _WIN32
    Password = getpass("Discord Password: ");
    #endif

    // Login!
    Discord_Login(Email, Password);

    // Increment Client State
    ClientState = 1;
}

/*
=====================
Client_WaitForCommand
=====================
Parse client input in a cheap way to print out available
commands
*/
void Client_WaitForCommand() {
    printf("-------------\n");
    printf("Client_WaitForCommand: actually do this soon lol\n");
    printf("lets print out client data!!!1!!\n");
    printf("-------------\n");
    printf("Client ID: %s\n", ClientUser.ID);
    printf("Client Username: %s\n", ClientUser.Username);
    printf("Client Avatar: %s\n", ClientUser.Avatar);
    printf("Client Discriminator: %s\n", ClientUser.Discriminator);
    printf("Client is Verified: %d\n", ClientUser.Verified);
    printf("Client allows NSFW: %d\n", ClientUser.NSFWAllowed);
    printf("Client has Multi-Factor Auth: %d\n", ClientUser.MFAEnabled);
    printf("-------------\n");

    ClientQuit = true;
}

/*
=====================
Client_Main
=====================
Main Client Loop
*/
void Client_Main() {
    // Set to 1 after Login
    ClientState = 0;

    // 'pretty' banner
    Client_Banner();

    // prompt for login
    while (!Discord_LoggedIn)
        Client_Login();

    while(!ClientQuit) {
        switch(ClientState) {
            // COMMAND LINE AREA THING
            case 1:
                Client_WaitForCommand();
                break;
        }
    }
}