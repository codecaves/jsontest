/*
    An experiment parsing JSON from a file using the json-c library. The
    library also has a json_object_from_file function which could be used 
    here instead. I did it this way because I'm typically parsing JSON from 
    a buffer that I filled from an HTTP request.

    For json_object_from_file example see:
    https://stackoverflow.com/questions/72584539/memory-leak-with-json-object-get-string
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#define MAX_FILE_SIZE 2048 // 2kb is fine for this test file
#define MAX_DESC_LEN 256
#define MAX_NAME_LEN 256

int main(void) {

    FILE *fp;
    char buffer[MAX_FILE_SIZE];

    struct json_object *parsed_json;
    struct json_object *json_data;
    struct json_object *json_metadata;
    struct json_object *json_message;
    struct json_object *json_description;
    struct json_object *json_name;
    struct json_object *json_symbol;

    struct Message {
        char description[MAX_DESC_LEN];
        char name[MAX_NAME_LEN];
        wchar_t *symbol; // TODO: need to figure out how to parse this
    } message;

    fp = fopen("test.json", "r");
    fread(buffer, MAX_FILE_SIZE, 1, fp);
    fclose(fp);

    /*
        Parse the JSON into the necessary objects.
    */
    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "data", &json_data);
    json_object_object_get_ex(parsed_json, "meta", &json_metadata);
    json_object_object_get_ex(json_data, "message", &json_message);
    json_object_object_get_ex(json_message, "description", &json_description);
    json_object_object_get_ex(json_message, "name", &json_name);
    json_object_object_get_ex(json_message, "symbol", &json_symbol);

    /*
        Copy the vaues into our Message struct. The advantage is that now we can pass around
        a pointer to this struct as needed.
    */
    strncpy(message.description, json_object_get_string(json_description), MAX_DESC_LEN);
    strncpy(message.name, json_object_get_string(json_name), MAX_NAME_LEN);

    printf("Name: %s\n", message.name);
    printf("Description: %s\n", message.description);

   return 0;
}