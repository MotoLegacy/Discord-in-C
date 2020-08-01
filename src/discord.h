// Client Information Struct
typedef struct {
    char* Token;
    char* Locale;
    char* Theme;
    char* Username;
    char* ID;
    char* Avatar;
    char* Discriminator;
    bool Verified;
    bool NSFWAllowed;
    bool MFAEnabled;
} DiscordClient_t;

extern DiscordClient_t ClientUser;

extern bool Discord_LoggedIn;

// Functions
extern void Discord_Login(char* email, char* password);
extern void Discord_FetchAccountData(char* JSONString);
extern void Discord_FetchUserData(char* Token);