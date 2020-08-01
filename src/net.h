// Reponse Struct
typedef struct {
    long Code;
    char* Header;
    char* Body;
} HTTPResponse_t;

// Functions
extern HTTPResponse_t Net_PostRequest(char* URL, char* PostData, char* AuthToken);
extern HTTPResponse_t Net_GetRequest(char* URL, char* AuthToken);