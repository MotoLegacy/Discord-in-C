//
// Discord.c - Functions pertaining to Discord(App) interaction (API).
//
#include <stdio.h>
#include <stdbool.h>

#include "defs.h"

// Our Client
DiscordClient_t ClientUser;

// Logged in Var
bool Discord_LoggedIn = false;

/*
=====================
Discord_FetchAccountData
=====================
Retrieves Account data from provided JSON string.
Format Ex: {"token": "YOUR_TOKEN", "user_settings": {"locale": "en-US", "theme": "dark"}}
*/
void Discord_FetchAccountData(char* JSONString)
{
    cJSON* JSONData = cJSON_Parse(JSONString);
    cJSON* UserData;
    cJSON* TokenData;
    cJSON* LocaleData;
    cJSON* ThemeData;

    // Token
    TokenData = cJSON_GetObjectItemCaseSensitive(JSONData, "token");
    ClientUser.Token = TokenData->valuestring;

    // User Settings
    UserData = cJSON_GetObjectItemCaseSensitive(JSONData, "user_settings");
    LocaleData = cJSON_GetObjectItemCaseSensitive(UserData, "locale");
    ThemeData = cJSON_GetObjectItemCaseSensitive(UserData, "theme");
    ClientUser.Locale = LocaleData->valuestring;
    ClientUser.Theme = ThemeData->valuestring;

    // Free cJSON Data
    //cJSON_Delete(JSONData);
}

/*
=====================
Discord_FetchUserData
=====================
Similar to Discord_FetchAccountData, but for more
basic information like Username, Avatar, etc.
Format:
{"id": "YOUR_ID", "username": "YOUR_USERNAME", "avatar": "YOUR_AVATAR", 
"discriminator": "YOUR_DISCRIMINATOR", "public_flags": 0, "flags": YOUR_FLAGS,
"email": "YOUR_EMAIL", "verified"L true, "locale": "YOUR_LOCALE", "nsfw_allowed": null,
"mfa_enabled": false, "phone": null}
*/
void Discord_FetchUserData(char* Token)
{
    // Send the GET Request
    char* DataURL = "https://discordapp.com/api/users/@me";
    HTTPResponse_t Response = Net_GetRequest(DataURL, Token);

    // Retrieve!
    cJSON* JSONData = cJSON_Parse(Response.Body);
    cJSON* IDData = cJSON_GetObjectItemCaseSensitive(JSONData, "id");
    cJSON* UsernameData = cJSON_GetObjectItemCaseSensitive(JSONData, "username");
    cJSON* AvatarData = cJSON_GetObjectItemCaseSensitive(JSONData, "avatar");
    cJSON* DiscriminatorData = cJSON_GetObjectItemCaseSensitive(JSONData, "discriminator");
    cJSON* VerifiedData = cJSON_GetObjectItemCaseSensitive(JSONData, "verified");
    cJSON* NSFWData = cJSON_GetObjectItemCaseSensitive(JSONData, "nsfw_allowed");
    cJSON* MFAData = cJSON_GetObjectItemCaseSensitive(JSONData, "mfa_enabled");
    ClientUser.ID = IDData->valuestring;
    ClientUser.Username = UsernameData->valuestring;
    ClientUser.Avatar = AvatarData->valuestring;
    ClientUser.Discriminator = DiscriminatorData->valuestring;
    ClientUser.Verified = ((VerifiedData->valueint == 1) ? true : false);
    ClientUser.NSFWAllowed = ((NSFWData->valueint == 1) ? true : false);
    ClientUser.MFAEnabled = ((MFAData->valueint == 1) ? true : false);
}

/*
=====================
Discord_Login
=====================
Send a POST request to the authentication URL
with entered username and password, to obtain
user authentication key (token)

TODO: 2FA Support
*/
void Discord_Login(char* email, char* password)
{
    // The URL we're authenticating to
    char* LoginURL = "https://discordapp.com/api/auth/login";

    // Our Post Request
    char Post[128];
    va_append(Post, "{ \"email\":\"%s\" , \"password\":\"%s\" }", email, password);

    // Fire Away!
    HTTPResponse_t Response = Net_PostRequest(LoginURL, Post, "");

    switch(Response.Code) {
        case 400:
            printf("Discord_Login: Invalid Credentials. Try Again?\n");
            break;
        case 200:
            // Notify Success
            printf("Discord_Login: Logged in Successfully.\n");

            // Retrieve Client Data
            Discord_FetchAccountData(Response.Body);
            Discord_FetchUserData(ClientUser.Token);

            // Alert Logged In
            Discord_LoggedIn = true;
            break;
        default:
            printf("Unknown Response: %ld\n", Response.Code);
            break;
    }
}