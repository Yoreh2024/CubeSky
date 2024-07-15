#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <yyjson.h>
#include <dirent.h>
#include <toml.h>

#define ROOT_DIR "/home/Yoreh/Code/CubeSky"
#define CONFIG_DIR ROOT_DIR"/config"
#define PROTOCOLS_DIR CONFIG_DIR"/protocols"