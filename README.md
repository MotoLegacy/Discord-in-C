# Discord in C (DiC)

## Notice
Custom Discord Clients are against ToS! Use DiC, or any other Custom Client, at your own risk. No DiC contributor is responsible for Account Termination, though it's highly unlikely this could occur.

## About
DiC is a Work-in-Progress Discord Client written in C. The End Goal is to have a fully-functional and easily port-able Discord Client for use with Desktop and Homebrew Platforms.

## Building
DiC uses general Makefile buildscripts. The Makefile is made to be modular and support easy building for different types of Client Interfaces. The currently available clients are
```
CLIENT_CLI: UNIX Command Line Interface
```
Build by running `make CLIENT_TYPE=1`
<br>
Example:
`make CLIENT_CLI=1`

## Regarding Security
All POST requests use the TLS protocol to prevent credentials from being discovered in plaintext. Any and all password dialogues hide input to protect over-the-shoulder password snatchin', and currently credentials are not stored anywhere on the File System.