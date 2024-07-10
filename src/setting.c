#include "common.h"
#include "setting.h"

struct ServerSettings ServerSetting={0};
struct ServerData ServerData={0};
struct ServerModules ServerModules={0};

void protocols_parse_config(const char* path){
    /*yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);

    DIR *dir;
    struct dirent *entry;
    struct stat st;

    dir = opendir(path);
    if (!dir) {
        perror("Failed to open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[256];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &st) == -1) {
            perror("Failed to get file status");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            protocols_parse_config(full_path);
        } else {
            //yyjson_doc *doc1 = yyjson_read_file(full_path, NULL, NULL, NULL);
            yyjson_val *root1 = yyjson_doc_get_root(doc1);
        }
    }

    // 关闭目录
    closedir(dir);

    
    yyjson_mut_doc_set_root(doc, root);
    yyjson_mut_obj_add_arr(doc, root, "name", NULL);
    yyjson_mut_obj_add_int(doc, root, "star", 4);
    int hits_arr[] = {2, 2, 1, 3};
    yyjson_mut_val *hits = yyjson_mut_arr_with_sint32(doc, hits_arr, 4);
    yyjson_mut_obj_add_val(doc, root, "hits", hits);
    const char *json = yyjson_mut_write(doc, 0, NULL);
    if (json) {
        printf("json: %s\n", json);
        free((void *)json);
    }
    yyjson_mut_doc_free(doc);*/
}

int server_setting(void){
    ServerSetting.ipv4_addr.s_addr = inet_addr("0.0.0.0");
    ServerSetting.port = htons(25565);
    ServerSetting.tcpserver_switch = 1;

    protocols_parse_config(PROTOCOLS_DIR);


}

int server_close(void){

}